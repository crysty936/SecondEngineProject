#include <assert.h>
#include "Renderer/Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLUtils.h"
#include "Core/EngineUtils.h"

Renderer RHI;

Renderer::Renderer()
= default;

Renderer::~Renderer()
= default;

eastl::shared_ptr<Window> Renderer::Init(const WindowProperties& inDefaultWindowProperties)
{
	const bool glfwSuccess = glfwInit() == GLFW_TRUE;

	ASSERT(glfwSuccess);

	glfwSetErrorCallback(GLUtils::GLFWErrorCallback);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	eastl::shared_ptr<Window> defaultWindow = CreateWindow(inDefaultWindowProperties);

	GLFWwindow* defaultWindowHandle = defaultWindow->GetHandle();

	glfwMakeContextCurrent(defaultWindowHandle);

	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;


	glViewport(0, 0, inDefaultWindowProperties.Width, inDefaultWindowProperties.Height);


	return defaultWindow;
}

void Renderer::Draw(const Window& inWindowToUse)
{
	glfwPollEvents();

	glfwSwapBuffers(inWindowToUse.GetHandle());
}

eastl::shared_ptr<Window> Renderer::CreateWindow(const WindowProperties& inWindowProperties) const
{
	GLFWwindow* newHandle = CreateNewWinowHandle(inWindowProperties);
	eastl::shared_ptr<Window> newWindow = eastl::make_shared<Window>(newHandle);

	return newWindow;
}


void Renderer::DestroyWindow(GLFWwindow* inWindowHandle) const
{
	glfwDestroyWindow(inWindowHandle);
}

GLFWwindow* Renderer::CreateNewWinowHandle(const WindowProperties& inWindowProperties) const
{
	GLFWwindow* newWindowHandle = glfwCreateWindow(inWindowProperties.Width, inWindowProperties.Height, inWindowProperties.Title.data(), nullptr, nullptr);

	return newWindowHandle;
}

