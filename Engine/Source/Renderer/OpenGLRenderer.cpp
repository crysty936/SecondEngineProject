#include <assert.h>
#include "Renderer/OpenGLRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLUtils.h"
#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"

OpenGLRenderer RHI;

OpenGLRenderer::OpenGLRenderer()
= default;

OpenGLRenderer::~OpenGLRenderer()
= default;

void OpenGLRenderer::Init(const WindowProperties& inDefaultWindowProperties)
{
	const bool glfwSuccess = glfwInit() == GLFW_TRUE;

	ASSERT(glfwSuccess);

	glfwSetErrorCallback(OpenGLUtils::GLFWErrorCallback);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	// Create new Window for data holding
	MainWindow = CreateWindow(inDefaultWindowProperties);

	// Set Context
	GLFWwindow* defaultWindowHandle = MainWindow->GetHandle();
	glfwMakeContextCurrent(defaultWindowHandle);
	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;
	glfwSetInputMode(defaultWindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glViewport(0, 0, inDefaultWindowProperties.Width, inDefaultWindowProperties.Height);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);

}

void OpenGLRenderer::Terminate()
{
	MainWindow.reset();
	glfwTerminate();
}

void OpenGLRenderer::Draw()
{
	glfwPollEvents();





	glfwSwapBuffers(MainWindow->GetHandle());

	CheckShouldCloseWindow(*MainWindow);
}

eastl::shared_ptr<OpenGLWindow> OpenGLRenderer::CreateWindow(const WindowProperties& inWindowProperties) const
{
	GLFWwindow* newHandle = CreateNewWinowHandle(inWindowProperties);
	eastl::shared_ptr<OpenGLWindow> newWindow = eastl::make_shared<OpenGLWindow>(newHandle);

	return newWindow;
}

void OpenGLRenderer::DestroyWindow(GLFWwindow* inWindowHandle) const
{
	glfwDestroyWindow(inWindowHandle);
}

void OpenGLRenderer::SetVSyncEnabled(const bool inEnabled)
{
	glfwSwapInterval(inEnabled);
}

GLFWwindow* OpenGLRenderer::CreateNewWinowHandle(const WindowProperties& inWindowProperties) const
{
	GLFWwindow* newWindowHandle = glfwCreateWindow(inWindowProperties.Width, inWindowProperties.Height, inWindowProperties.Title.data(), nullptr, nullptr);

	return newWindowHandle;
}

void OpenGLRenderer::CheckShouldCloseWindow(const OpenGLWindow& inWindow)
{
	if (glfwWindowShouldClose(inWindow.GetHandle()))
	{
		StopEngineRunning();
	}
}

