#include <assert.h>
#include "Renderer/ForwardRenderer.h"
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
#include "Renderer/RHI/Resources/RenderDataContainer.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/Drawable/MirrorQuad.h"
#include "Core/ObjectCreation.h"
#include "Renderer/Drawable/DepthMaterial.h"
#include "Core/WindowsPlatform.h"

#include "InputSystem/InputType.h"
#include "Window/WindowsWindow.h"
#include "InputSystem/InputSystem.h"

#include "Renderer/RHI/RHI.h"
#include "Renderer/RHI/Resources/RHIShader.h"
#include "Renderer/RHI/Resources/RHITexture.h"

constexpr glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);
constexpr glm::vec3 lightPos(-10.0f, 10.0f, -1.0f);

const uint32_t SHADOW_WIDTH = 4096;
const uint32_t SHADOW_HEIGHT = 4096;

static std::mutex RenderCommandsMutex;
static std::mutex LoadQueueMutex;
static std::mutex GetVAOMutex;
static std::condition_variable LoadQueueCondition;

// void LoaderFunc(GLFWwindow* inLoadingThreadContext)
// {
// 	while (Engine->IsRunning())
// 	{
// 		eastl::queue<RenderingLoadCommand>& loadQueue = OpenGLRenderer::GetRHI().GetLoadQueue();
// 		std::unique_lock<std::mutex> lock{ LoadQueueMutex };
// 		LoadQueueCondition.wait(lock, [&] {return !loadQueue.empty(); });
// 
// 		RenderingLoadCommand newCommand = loadQueue.front();
// 		loadQueue.pop();
// 
// 		lock.unlock();
// 
// 		// TODO: Make work without glfw
// 		//glfwMakeContextCurrent(inLoadingThreadContext);
// 		glEnable(GL_DEBUG_OUTPUT);
// 		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
// 		//glDebugMessageCallback(GLDebugCallback, nullptr);
// 
// 		newCommand.LoadDel.Execute(newCommand.ModelPath, newCommand.Parent);
// 
// 		//glfwMakeContextCurrent(nullptr);
// 	}
// }

ForwardRenderer::ForwardRenderer(const WindowProperties& inMainWindowProperties)
{
	SetViewportSizeToMain();

	RHI::Instance->ClearColor(ClearColor);

	// Set the default uniforms
	SetupBaseUniforms();


// 	// Create the shadow map framebuffer
// 	glGenFramebuffers(1, &RHI->ShadowMapBuffer);
// 	glBindFramebuffer(GL_FRAMEBUFFER, RHI->ShadowMapBuffer);
// 
// 	RHI->ShadowBufferTex = eastl::make_shared<OpenGLDepthMap>("ShadowMap");
// 	RHI->ShadowBufferTex->Init();
// 
// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, RHI->ShadowBufferTex->TexHandle, 0);
// 
// 	glDrawBuffer(GL_NONE);
// 	glReadBuffer(GL_NONE);
// 
// 	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
// 
// 	// Bind the default frame buffer
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ForwardRenderer::~ForwardRenderer() = default;

void ForwardRenderer::Init(const WindowProperties & inMainWindowProperties)
{
	Instance = new ForwardRenderer{ inMainWindowProperties };
}

void ForwardRenderer::Terminate()
{
	//glDeleteBuffers(1, &Instance->AuxiliarFrameBuffer);

	ASSERT(Instance);
	delete Instance;
}

void ForwardRenderer::Draw()
{
	UpdateUniforms();

	//DrawShadowMap();

	SetupBaseUniforms();
	UpdateUniforms();

	RHI::Instance->ClearBuffers();

	//DrawSkybox();
	RenderCommandsMutex.lock();
	DrawCommands(MainCommands);
	RenderCommandsMutex.unlock();

	RHI::Instance->SwapBuffers();

}

void ForwardRenderer::DrawSkybox()
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

void ForwardRenderer::DrawShadowMap()
{
// 	// Cull front face to solve Peter Panning
// 	//glCullFace(GL_FRONT);
// 	glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapBuffer);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
// 
// 	SetDrawMode(EDrawMode::DEPTH);
// 	RHIBase::RHI->SetViewportSize(SHADOW_WIDTH, SHADOW_HEIGHT);
// 
// 	//const glm::vec3 lightLoc = LightSource->GetAbsoluteTransform().Translation;
// 	const glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
// 	const float near_plane = 1.f;
// 	const float far_plane = 20.f;
// 	const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
// 
// 	//UniformsCache["projection"] = lightProjection;
// 
// 	const glm::mat4 lightSpaceMatrix = lightProjection * lightView;
// 
// 	UniformsCache["lightSpaceMatrix"] = lightSpaceMatrix;
// 
// 	RenderCommandsMutex.lock();
// 	DrawCommands(MainCommands);
// 	RenderCommandsMutex.unlock();
// 
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 
// 	SetViewportSizeToMain();
// 	SetDrawMode(EDrawMode::NORMAL);
// 
// 	// Reset to default
// 	//glCullFace(GL_BACK);
}

void ForwardRenderer::SetupBaseUniforms()
{
	// By default, use a D3D11 projection matrix.
	// Note: glm is RH but uses a sneaky minus to change the handedness of the output to LH (how OpenGL is)
	glm::mat4 projection = glm::perspectiveRH_ZO(glm::radians(45.0f), static_cast<float>(Engine->GetMainWindow().GetProperties().Width) / static_cast<float>(Engine->GetMainWindow().GetProperties().Height), 0.1f, 1000.0f);
	RHI::Instance->PrepareProjectionForRendering(projection);

	UniformsCache["projection"] = projection;
}

void ForwardRenderer::UpdateUniforms()
{
	const glm::mat4 view = SceneManager::Get().GetCurrentScene().CurrentCamera->GetLookAt();
	UniformsCache["view"] = view;
}

void ForwardRenderer::DrawCommands(const eastl::vector<RenderCommand>&inCommands)
{
	for (const RenderCommand& renderCommand : inCommands)
	{
		DrawCommand(renderCommand);
	}
}

void ForwardRenderer::DrawCommand(const RenderCommand & inCommand)
{
	const bool parentValid = !inCommand.Parent.expired();
	if (!ENSURE(parentValid))
	{
		return;
	}

	const eastl::shared_ptr<const DrawableObject> parent = inCommand.Parent.lock();
	const eastl::shared_ptr<RenderMaterial> material = GetMaterial(inCommand);
	const eastl::shared_ptr<RenderDataContainer>& dataContainer = inCommand.DataContainer;

	if (!parent->IsVisible())
	{
		return;
	}

	RHI::Instance->BindVertexBuffer(*(dataContainer->VBuffer));
	RHI::Instance->BindShader(*(material->Shader));

	material->ResetUniforms();

	UniformsCache["model"] = parent->GetModelMatrix();

	int32_t i = 0;
	for (i = 0; i < material->DiffuseTextures.size(); ++i)
	{
		eastl::shared_ptr<RHITexture2D>& tex = material->DiffuseTextures[i];
		RHI::Instance->BindTexture2D(*tex, i);
	}

	// Shadows
	//
	//ShadowBufferTex->Bind(i);
	//
	//GlobalRHI->UniformsCache["ShadowMap"] = uint32_t(i);
	//GlobalRHI->UniformsCache["LightPos"] = lightPos;

	const uint32_t indicesCount = dataContainer->VBuffer->GetIndicesCount();

	material->SetUniforms(UniformsCache);
	material->BindBuffers();

	switch (inCommand.DrawType)
	{
	case EDrawCallType::DrawElements:
	{
		RHI::Instance->DrawElements(indicesCount);

		break;
	}
	case EDrawCallType::DrawArrays:
	{
		//glDrawArrays(GL_TRIANGLES, 0, indicesCount);
		break;
	}
	}

	RHI::Instance->UnbindVertexBuffer(*(dataContainer->VBuffer));

	for (i = 0; i < material->DiffuseTextures.size(); ++i)
	{
		eastl::shared_ptr<RHITexture2D>& tex = material->DiffuseTextures[i];
		RHI::Instance->UnbindTexture2D(*tex, i);
	}

	// Shadows
	//
	//ShadowBufferTex->Unbind(i);
	//

	material->UnbindBuffers();
	RHI::Instance->UnbindShader(*(material->Shader));
}

eastl::shared_ptr<RenderMaterial> ForwardRenderer::GetMaterial(const RenderCommand & inCommand) const
{
	switch (DrawMode)
	{
	case EDrawMode::NORMAL:
	{
		return inCommand.Material;
	}
	//case EDrawMode::DEPTH:
	//{
// 		MaterialsManager& matManager = MaterialsManager::Get();
// 		bool materialExists = false;
// 		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial<DepthMaterial>("depth_material", materialExists);
// 
// 		if (!materialExists)
// 		{
// 			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicDepthVertexShader.glsl", "../Data/Shaders/BasicDepthFragmentShader.glsl");
// 		}
// 
// 		return depthMaterial;
// 	}
// 	case EDrawMode::DEPTH_VISUALIZE:
// 	{
// 		MaterialsManager& matManager = MaterialsManager::Get();
// 		bool materialExists = false;
// 		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial("visualize_depth_material", materialExists);
// 
// 		if (!materialExists)
// 		{
// 			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/VisualizeDepthFragmentShader.glsl");
// 		}
// 
// 		return depthMaterial;
// 	}
// 	case EDrawMode::OUTLINE:
// 	{
// 		MaterialsManager& matManager = MaterialsManager::Get();
// 		bool materialExists = false;
// 		eastl::shared_ptr<RenderMaterial> outlineMaterial = matManager.GetOrAddMaterial("outline_material", materialExists);
// 
// 		if (!materialExists)
// 		{
// 			outlineMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/OutlineFragmentShader.glsl");
// 		}
// 
// 		return outlineMaterial;
	//}
	}

	return { nullptr };
}

void ForwardRenderer::SetVSyncEnabled(const bool inEnabled)
{
	//glfwSwapInterval(inEnabled);
}

void ForwardRenderer::AddMirrorCommand(const RenderCommand & inCommand)
{
	// 	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	// 	MirrorCommands.push_back(inCommand);
}

void ForwardRenderer::AddCommand(const RenderCommand & inCommand)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MainCommands.push_back(inCommand);
}

void ForwardRenderer::AddCommands(eastl::vector<RenderCommand> inCommands)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
#ifdef _DEBUG
	for (RenderCommand& command : inCommands)
	{
		ASSERT(command.DataContainer);
		ASSERT(command.Material);
	}
#endif

	MainCommands.insert(MainCommands.end(), inCommands.begin(), inCommands.end());
}

void ForwardRenderer::AddRenderLoadCommand(const RenderingLoadCommand & inCommand)
{
	std::unique_lock<std::mutex> lock{ LoadQueueMutex };

	LoadQueue.push(inCommand);
	LoadQueueCondition.notify_one();
}

bool ForwardRenderer::GetOrCreateContainer(const eastl::string& inInstanceName, OUT eastl::shared_ptr<RenderDataContainer>& outContainer)
{
	ASSERT(!inInstanceName.empty());
	std::lock_guard<std::mutex> uniqueMutex(GetVAOMutex);
	//GetVAOMutex.lock(); // TODO: Why does this not work?

	using iterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<RenderDataContainer>>::iterator;
	const iterator& containerIter = RenderDataContainerMap.find(inInstanceName);
	const bool materialExists = containerIter != RenderDataContainerMap.end();

	if (materialExists)
	{
		outContainer = (*containerIter).second;

		return true;
	}

	eastl::shared_ptr<RenderDataContainer> newContainer = eastl::make_shared<RenderDataContainer>();
	RenderDataContainerMap[inInstanceName] = newContainer;
	outContainer = newContainer;

	//GetVAOMutex.unlock();

	return false;
}

void ForwardRenderer::SetViewportSizeToMain()
{
	const WindowsWindow& currentWindow = Engine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();
	RHI::Instance->SetViewportSize(props.Width, props.Height);
}

