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
#include "../RenderableObject.h"

#define CLEAR_COLOR 0.3f, 0.5f, 1.f, 0.4f

OpenGLRenderer* RHI = nullptr;

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
	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;
	glfwSetInputMode(mainWindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, inDefaultWindowProperties.Width, inDefaultWindowProperties.Height);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);
	glClearColor(CLEAR_COLOR);
}

OpenGLRenderer::~OpenGLRenderer()
= default;

void OpenGLRenderer::Init(const WindowProperties & inDefaultWindowProperties)
{
	RHI = new OpenGLRenderer{ inDefaultWindowProperties };
}

void OpenGLRenderer::Terminate()
{
	RHI->MainWindow.reset();
	glfwTerminate();

	ASSERT(RHI);
	delete RHI;
}

void OpenGLRenderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	SceneManager& sceneMan = SceneManager::Get();
	Scene& currentScene = sceneMan.GetCurrentScene();
	eastl::vector<eastl::shared_ptr<ITickableObject>>& sceneObjects = currentScene.SceneObjects;

	for (eastl::shared_ptr<ITickableObject>& object : sceneObjects)
	{
		ITickableObject* tickable = object.get();
		RenderableObject* renderable = dynamic_cast<RenderableObject*>(tickable);

		if (renderable)
		{
			uint32_t indicesCount = renderable->ObjectVAO.GetVertexBuffer().GetIndicesCount();

			renderable->Shader.Bind();
			renderable->ObjectVAO.Bind();
			glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

			renderable->Shader.UnBind();
			renderable->ObjectVAO.Unbind();
		}
	}

	// Get matrices from Camera, issue Rendering commands

	CheckShouldCloseWindow(*MainWindow);
	glfwSwapBuffers(MainWindow->GetHandle());
}

eastl::unique_ptr<OpenGLWindow> OpenGLRenderer::CreateWindow(const WindowProperties & inWindowProperties) const
{
	GLFWwindow* newHandle = CreateNewWinowHandle(inWindowProperties);
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

GLFWwindow* OpenGLRenderer::CreateNewWinowHandle(const WindowProperties & inWindowProperties) const
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

