#include <assert.h>
#include "Renderer/Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "GLUtils.h"

Renderer RHI;

Renderer::Renderer()
= default;

Renderer::~Renderer()
= default;

void Renderer::Init()
{
	const bool glfwSuccess = glfwInit() == GLFW_TRUE;

	assert(glfwSuccess);

	glfwSetErrorCallback(GLUtils::GLFWErrorCallback);
	
	// Test to see if gladLoadGLLoader can be used without setting the current glfw context



	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;

}

void Renderer::Draw(const Window& inWindowToUse)
{

	GLFWwindow* windowHandle = inWindowToUse.GetHandle<GLFWwindow>();

	glfwMakeContextCurrent(windowHandle);





}

void* Renderer::CreateWindow(const int32_t inWidth, const int32_t inHeight, const char* inWindowTitle)
{
	GLFWwindow* newWindow = glfwCreateWindow(inWidth, inHeight, inWindowTitle, nullptr, nullptr);


	return newWindow;
}

void Renderer::DestroyWindow(void* inWindowHandle) const
{
	GLFWwindow* window = reinterpret_cast<GLFWwindow*>(inWindowHandle);

	glfwDestroyWindow(window);
}

