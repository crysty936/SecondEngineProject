#include <assert.h>
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "glad/glad.h"
#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include "Entity/Entity.h"
#include "Renderer/Drawable/Drawable.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/Material/RenderMaterial.h"
#include "Buffer/VertexArrayObject.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/Drawable/MirrorQuad.h"
#include "Core/ObjectCreation.h"
#include "OpenGLDepthMap.h"
#include "OpenGLRenderTexture.h"
#include "Renderer/Drawable/DepthMaterial.h"
#include "Core/WindowsPlatform.h"

#include <windows.h>
#include "InputSystem/InputType.h"
#include "Window/WindowsWindow.h"
#include "InputSystem/InputSystem.h"

#if WITH_GLFW
#include "GLFW/glfw3.h"
#endif
#include "Renderer/RHI/RHIBase.h"

constexpr glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);
constexpr glm::vec3 lightPos(-10.0f, 10.0f, -1.0f);

const uint32_t SHADOW_WIDTH = 4096;
const uint32_t SHADOW_HEIGHT = 4096;

static std::mutex RenderCommandsMutex;
static std::mutex LoadQueueMutex;
static std::mutex GetVAOMutex;
static std::condition_variable LoadQueueCondition;

void LoaderFunc(GLFWwindow* inLoadingThreadContext)
{
	while (Engine->IsRunning())
	{
		eastl::queue<RenderingLoadCommand>& loadQueue = OpenGLRenderer::GetRHI().GetLoadQueue();
		std::unique_lock<std::mutex> lock{ LoadQueueMutex };
		LoadQueueCondition.wait(lock, [&] {return !loadQueue.empty(); });

		RenderingLoadCommand newCommand = loadQueue.front();
		loadQueue.pop();

		lock.unlock();

		// TODO: Make work without glfw
		//glfwMakeContextCurrent(inLoadingThreadContext);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//glDebugMessageCallback(GLDebugCallback, nullptr);

		newCommand.LoadDel.Execute(newCommand.ModelPath, newCommand.Parent);

		//glfwMakeContextCurrent(nullptr);
	}
}

OpenGLRenderer::OpenGLRenderer(const WindowProperties& inMainWindowProperties)
{
	SetViewportSizeToMain();

	RHIBase::RHI->ClearColor(ClearColor);

	// Set the default uniforms
	SetupBaseUniforms();
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Init(const WindowProperties & inMainWindowProperties)
{
	GLRenderer = new OpenGLRenderer{ inMainWindowProperties };
}

void OpenGLRenderer::Terminate()
{

#if WITH_GLFW
	GLRenderer->GLWindow.reset();
	glfwTerminate();
#endif;

	glDeleteBuffers(1, &GLRenderer->AuxiliarFrameBuffer);

	ASSERT(GLRenderer);
	delete GLRenderer;
}

void OpenGLRenderer::Draw()
{
	UpdateUniforms();

	//DrawShadowMap();

	SetupBaseUniforms();
	UpdateUniforms();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//DrawSkybox();
	RenderCommandsMutex.lock();
	DrawCommands(MainCommands);
	RenderCommandsMutex.unlock();

	RHIBase::RHI->SwapBuffers();

#if WITH_GLFW
	glfwSwapBuffers(GLWindow->GetHandle());
#endif

}

void OpenGLRenderer::DrawSkybox()
{
// 	if (!MainSkyboxCommand.Parent.lock())
// 	{
// 		return;
// 	}
// 
// 	if (!MainSkyboxCommand.VAO->bReadyForDraw)
// 	{
// 		MainSkyboxCommand.VAO->SetupState();
// 	}
// 
// 	glDepthFunc(GL_LEQUAL);
// 	DrawCommand(MainSkyboxCommand);
// 	glDepthFunc(GL_LESS);
}

void OpenGLRenderer::DrawShadowMap()
{
	// Cull front face to solve Peter Panning
	//glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SetDrawMode(EDrawMode::DEPTH);
	RHIBase::RHI->SetViewportSize(SHADOW_WIDTH, SHADOW_HEIGHT);

	//const glm::vec3 lightLoc = LightSource->GetAbsoluteTransform().Translation;
	const glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const float near_plane = 1.f;
	const float far_plane = 20.f;
	const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	//UniformsCache["projection"] = lightProjection;

	const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	UniformsCache["lightSpaceMatrix"] = lightSpaceMatrix;

	RenderCommandsMutex.lock();
	DrawCommands(MainCommands);
	RenderCommandsMutex.unlock();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SetViewportSizeToMain();
	SetDrawMode(EDrawMode::NORMAL);

	// Reset to default
	//glCullFace(GL_BACK);
}

void OpenGLRenderer::SetupBaseUniforms()
{
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Engine->GetMainWindow().GetProperties().Width) / static_cast<float>(Engine->GetMainWindow().GetProperties().Height), 0.1f, 1000.0f);
	UniformsCache["projection"] = projection;
}

void OpenGLRenderer::UpdateUniforms()
{
	const glm::mat4 view = SceneManager::Get().GetCurrentScene().CurrentCamera->GetLookAt();
	UniformsCache["view"] = view;
}

void OpenGLRenderer::DrawCommands(const eastl::vector<RenderCommand>&inCommands)
{
	for (const RenderCommand& renderCommand : inCommands)
	{
		DrawCommand(renderCommand);
	}
}

void OpenGLRenderer::DrawCommand(const RenderCommand & inCommand)
{
	const bool parentValid = !inCommand.Parent.expired();
	if (!ENSURE(parentValid))
	{
		return;
	}

	const eastl::shared_ptr<const DrawableObject> parent = inCommand.Parent.lock();
	const eastl::shared_ptr<RenderMaterial> material = GetMaterial(inCommand);
	const eastl::shared_ptr<VertexArrayObject>& vao = inCommand.VAO;

	if (!parent->IsVisible())
	{
		return;
	}

	// TODO: Abstract the model and parent dependent uniforms (like the Model Matrix) to be present in the render command 
	// and updated only if dirty

	// Deffered VAO initialization on the Main Rendering Thread
	//if (!vao->bReadyForDraw)
	{
		//vao->SetupState();
		vao->VBuffer.Bind();

		const VertexBufferLayout& layout = vao->VBuffer.GetLayout();
		const eastl::vector<VertexLayoutProperties>& props = layout.GetProperties();

		size_t offset = 0;
		for (int32_t i = 0; i < props.size(); i++)
		{
			const VertexLayoutProperties& prop = props[i];

			void* offsetPtr = reinterpret_cast<void*>(offset);
			uint32_t glType = 0;

			switch (prop.Type)
			{
			case VertexPropertyType::UInt:
				glType = GL_UNSIGNED_INT;
				break;
			case VertexPropertyType::Float:
				glType = GL_FLOAT;
				break;
			}

			glVertexAttribPointer(i, prop.Count, glType, prop.bNormalized, layout.GetStride(), offsetPtr);
			glEnableVertexAttribArray(i);

			offset += prop.Count * prop.GetSizeOfType();
		}
	}

	material->Shader.Bind();
	material->ResetUniforms();

	UniformsCache["model"] = parent->GetModelMatrix();

	int32_t i = 0;
	for (i = 0; i < material->Textures.size(); ++i)
	{
		eastl::shared_ptr<OpenGLTexture>& tex = material->Textures[i];
		tex->Bind(i);
	}

	// Shadows
	//
	//ShadowBufferTex->Bind(i);
	//
	//GlobalRHI->UniformsCache["ShadowMap"] = uint32_t(i);
	//GlobalRHI->UniformsCache["LightPos"] = lightPos;

	const uint32_t indicesCount = vao->VBuffer.GetIndicesCount();
	//vao->Bind();


	material->SetUniforms(UniformsCache);

	switch (inCommand.DrawType)
	{
	case EDrawCallType::DrawElements:
	{
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		break;
	}
	case EDrawCallType::DrawArrays:
	{
		glDrawArrays(GL_TRIANGLES, 0, indicesCount);
		break;
	}
	}

	//vao->Unbind();
	vao->VBuffer.Unbind();

	for (i = 0; i < material->Textures.size(); ++i)
	{
		eastl::shared_ptr<OpenGLTexture>& tex = material->Textures[i];
		tex->Unbind(i);
	}

	// Shadows
	//
	//ShadowBufferTex->Unbind(i);
	//

	material->Shader.UnBind();
}

eastl::shared_ptr<RenderMaterial> OpenGLRenderer::GetMaterial(const RenderCommand & inCommand) const
{
	switch (DrawMode)
	{
	case EDrawMode::NORMAL:
	{
		return inCommand.Material;
	}
	case EDrawMode::DEPTH:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial<DepthMaterial>("depth_material", materialExists);

		if (!materialExists)
		{
			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicDepthVertexShader.glsl", "../Data/Shaders/BasicDepthFragmentShader.glsl");
		}

		return depthMaterial;
	}
	case EDrawMode::DEPTH_VISUALIZE:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial("visualize_depth_material", materialExists);

		if (!materialExists)
		{
			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/VisualizeDepthFragmentShader.glsl");
		}

		return depthMaterial;
	}
	case EDrawMode::OUTLINE:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> outlineMaterial = matManager.GetOrAddMaterial("outline_material", materialExists);

		if (!materialExists)
		{
			outlineMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/OutlineFragmentShader.glsl");
		}

		return outlineMaterial;
	}
	}

	return { nullptr };
}

void OpenGLRenderer::SetVSyncEnabled(const bool inEnabled)
{
	//glfwSwapInterval(inEnabled);
}

void OpenGLRenderer::AddMirrorCommand(const RenderCommand & inCommand)
{
	// 	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	// 	MirrorCommands.push_back(inCommand);
}

void OpenGLRenderer::AddCommand(const RenderCommand & inCommand)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MainCommands.push_back(inCommand);
}

void OpenGLRenderer::AddCommands(eastl::vector<RenderCommand> inCommands)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MainCommands.insert(MainCommands.end(), inCommands.begin(), inCommands.end());
}

void OpenGLRenderer::AddRenderLoadCommand(const RenderingLoadCommand & inCommand)
{
	std::unique_lock<std::mutex> lock{ LoadQueueMutex };

	LoadQueue.push(inCommand);
	LoadQueueCondition.notify_one();
}

bool OpenGLRenderer::GetOrCreateVAO(const eastl::string & inVAOId, OUT eastl::shared_ptr<VertexArrayObject>&outVAO)
{
	ASSERT(!inVAOId.empty());
	std::lock_guard<std::mutex> uniqueMutex(GetVAOMutex);
	//GetVAOMutex.lock(); // TODO: Why does this not work?

	using iterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<VertexArrayObject>>::iterator;
	const iterator& vaoIter = VAOs.find(inVAOId);
	const bool materialExists = vaoIter != VAOs.end();

	if (materialExists)
	{
		outVAO = (*vaoIter).second;

		return true;
	}

	eastl::shared_ptr<VertexArrayObject> newVAO = eastl::make_shared<VertexArrayObject>();
	VAOs[inVAOId] = newVAO;
	outVAO = newVAO;

	//GetVAOMutex.unlock();

	return false;
}

void OpenGLRenderer::SetViewportSizeToMain()
{
	const WindowsWindow& currentWindow = Engine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();
	RHIBase::RHI->SetViewportSize(props.Width, props.Height);
}

