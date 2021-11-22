#include <assert.h>
#include "Renderer/OpenGLRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLUtils.h"
#include "Core/EngineUtils.h"

OpenGLRenderer RHI;

OpenGLRenderer::OpenGLRenderer()
= default;

OpenGLRenderer::~OpenGLRenderer()
= default;

eastl::shared_ptr<OpenGLWindow> OpenGLRenderer::Init(const WindowProperties& inDefaultWindowProperties)
{
	const bool glfwSuccess = glfwInit() == GLFW_TRUE;

	ASSERT(glfwSuccess);

	glfwSetErrorCallback(OpenGLUtils::GLFWErrorCallback);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	// Create new Window for data holding
	eastl::shared_ptr<OpenGLWindow> defaultWindow = CreateWindow(inDefaultWindowProperties);

	GLFWwindow* defaultWindowHandle = defaultWindow->GetHandle();

	glfwMakeContextCurrent(defaultWindowHandle);

	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;

	glViewport(0, 0, inDefaultWindowProperties.Width, inDefaultWindowProperties.Height);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);



	return defaultWindow;
}

void OpenGLRenderer::Draw(const OpenGLWindow& inWindowToUse)
{
	glfwPollEvents();

	glfwSwapBuffers(inWindowToUse.GetHandle());
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

