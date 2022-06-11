#include <assert.h>
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLUtils.h"
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

OpenGLRenderer* RHI = nullptr;
static std::mutex RenderCommandsMutex;
static std::mutex LoadQueueMutex;
static std::mutex GetVAOMutex;
static std::condition_variable LoadQueueCondition;

constexpr glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);

void LoaderFunc(GLFWwindow* inLoadingThreadContext)
{
	while (Engine->IsRunning())
	{
		eastl::queue<RenderingLoadCommand>& loadQueue = RHI->GetLoadQueue();
		std::unique_lock<std::mutex> lock{ LoadQueueMutex };
		LoadQueueCondition.wait(lock, [&] {return !loadQueue.empty(); });

		RenderingLoadCommand newCommand = loadQueue.front();
		loadQueue.pop();

		lock.unlock();

		glfwMakeContextCurrent(inLoadingThreadContext);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);

		newCommand.LoadDel.Execute(newCommand.ModelPath, newCommand.Parent);

		glfwMakeContextCurrent(nullptr);
	}
}

OpenGLRenderer::OpenGLRenderer(const WindowProperties& inDefaultWindowProperties)
{
	const bool glfwSuccess = glfwInit() == GLFW_TRUE;

	ASSERT(glfwSuccess);

	glfwSetErrorCallback(OpenGLUtils::GLFWErrorCallback);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	// Create new Window for data holding
	MainWindow = CreateWindow(inDefaultWindowProperties);

	// Set Context
	GLFWwindow* mainWindowHandle = MainWindow->GetHandle();
	glfwMakeContextCurrent(mainWindowHandle);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* loadingThreadContext = glfwCreateWindow(1, 1, "Loading Thread Window", nullptr, mainWindowHandle);

	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;
	glfwSetInputMode(mainWindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, inDefaultWindowProperties.Width, inDefaultWindowProperties.Height);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);

	// Set the default uniforms
	SetupBaseUniforms();

	// Create the loading thread
	std::thread(LoaderFunc, loadingThreadContext).detach();
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Init(const WindowProperties & inDefaultWindowProperties)
{
	RHI = new OpenGLRenderer{ inDefaultWindowProperties };

	// Setup secondary framebuffer

	// Create the frame buffer
	glGenFramebuffers(1, &RHI->FrameBufferHandle);

	glBindFramebuffer(GL_FRAMEBUFFER, RHI->FrameBufferHandle);

	const WindowProperties& windowProps = RHI->MainWindow->GetProperties();

	// Create a stencil and depth render buffer object for the frame buffer
	uint32_t rbo; // render buffer object
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowProps.Width, windowProps.Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach the rbo to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	// Bind the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::Terminate()
{
	RHI->MainWindow.reset();
	glfwTerminate();

	glDeleteBuffers(1, &RHI->FrameBufferHandle);


	ASSERT(RHI);
	delete RHI;
}

void OpenGLRenderer::Draw()
{
 	//DrawMirrorStuff();

 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 	SetupBaseUniforms();
 	UpdateUniforms();
 
	DrawCommands(MainCommands);
 	DrawCommands(MirrorCommands);

 	CheckShouldCloseWindow(*MainWindow);
 	glfwSwapBuffers(MainWindow->GetHandle());
}

void OpenGLRenderer::DrawMirrorStuff()
{
	// First draw in the secondary frame buffer for the mirror
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferHandle);
	
	for (const RenderCommand& mirrorCommand : MirrorCommands)
	{
		// Clear the FBO
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//const OpenGLTexture& tex = mirrorCommand.Material->Textures[0];
		OpenGLTexture tex;
		// Attach the texture to the FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.TexHandle, 0);
		// Set the color in the texture to ClearColor
		glClearTexImage(tex.TexHandle, 0, GL_RGBA, GL_FLOAT, &ClearColor);

		// Set the mirror projection and view uniforms
 		const glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 1000.0f);
 		UniformsCache["projection"] = projection;
 
 		const eastl::shared_ptr<const DrawableObject> parent = mirrorCommand.Parent.lock();
		Transform parentTransform = parent->GetAbsoluteTransform();
 		const glm::mat4 inverse = glm::inverse(parent->GetAbsoluteTransform().GetMatrix());
 		UniformsCache["view"] = inverse;

		DrawCommands(MainCommands);

		if (!mirrorCommand.Material->Textures.empty())
		{
			mirrorCommand.Material->Textures.pop_back();
		}

		mirrorCommand.Material->Textures.push_back(std::move(tex));

		// Detach the current texture from the buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	}
}

void OpenGLRenderer::SetupBaseUniforms()
{
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(MainWindow->GetProperties().Width) / static_cast<float>(MainWindow->GetProperties().Height), 0.1f, 1000.0f);
	UniformsCache["projection"] = projection;
}

void OpenGLRenderer::UpdateUniforms()
{
	const glm::mat4 view = SceneManager::Get().GetCurrentScene().CurrentCamera->GetLookAt();
	UniformsCache["view"] = view;
}

void OpenGLRenderer::DrawCommands(const eastl::vector<RenderCommand>& inCommands)
{
	RenderCommandsMutex.lock();

	for (const RenderCommand& renderCommand : inCommands)
	{
		const bool parentValid = !renderCommand.Parent.expired();
		if (!ENSURE(parentValid))
		{
			continue;
		}

		DrawCommand(renderCommand);
	}

	RenderCommandsMutex.unlock();
}

void OpenGLRenderer::DrawCommand(const RenderCommand & inCommand)
{
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
	if (!vao->bReadyForDraw)
	{
		vao->SetupState();
	}

	material->Shader.Bind();

	UniformsCache["model"] = parent->GetModelMatrix();

	for (const OpenGLTexture& tex : material->Textures)
	{
		tex.Bind();
	}

	const uint32_t indicesCount = vao->VBuffer.GetIndicesCount();
	vao->Bind();
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

	vao->Unbind();

	for (const OpenGLTexture& tex : material->Textures)
	{
		tex.Unbind();
	}

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
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial("depth_material", materialExists);

		if (!materialExists)
		{
			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/DepthFragmentShader.glsl");
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

eastl::unique_ptr<OpenGLWindow> OpenGLRenderer::CreateWindow(const WindowProperties & inWindowProperties) const
{
	GLFWwindow* newHandle = CreateNewWindowHandle(inWindowProperties);
	eastl::unique_ptr<OpenGLWindow> newWindow = eastl::make_unique<OpenGLWindow>(newHandle);

	return newWindow;
}

void OpenGLRenderer::DestroyWindow(GLFWwindow * inWindowHandle) const
{
	glfwDestroyWindow(inWindowHandle);
}

void OpenGLRenderer::SetVSyncEnabled(const bool inEnabled)
{
	glfwSwapInterval(inEnabled);
}

void OpenGLRenderer::AddMirrorCommand(const RenderCommand& inCommand)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MirrorCommands.push_back(inCommand);
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

GLFWwindow* OpenGLRenderer::CreateNewWindowHandle(const WindowProperties & inWindowProperties) const
{
	GLFWwindow* newWindowHandle = glfwCreateWindow(inWindowProperties.Width, inWindowProperties.Height, inWindowProperties.Title.data(), nullptr, nullptr);

	return newWindowHandle;
}

void OpenGLRenderer::CheckShouldCloseWindow(const OpenGLWindow & inWindow)
{
	if (glfwWindowShouldClose(inWindow.GetHandle()))
	{
		StopEngineRunning();
	}
}

