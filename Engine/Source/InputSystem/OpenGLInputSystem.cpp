#include "InputSystem/OpenGlInputSystem.h"
#include "Window/OpenGLWindow.h"

OpenGLInputSystem* OpenGLInputSystem::Instance;

OpenGLInputSystem::OpenGLInputSystem() = default;
OpenGLInputSystem::~OpenGLInputSystem() = default;

void OpenGLInputSystem::OnKeyPressedTest(int32_t keycode, int32_t scanCode, int32_t action, int32_t mods)
{
	Logger::Get().Print("Key input received.");
}

void KeyCallback(GLFWwindow*, int32_t keycode, int32_t scanCode, int32_t action, int32_t mods)
{
	OpenGLInputSystem& instance = OpenGLInputSystem::Get();

	instance.OnKeysPressed.Invoke(keycode, scanCode, action, mods);
}

void OpenGLInputSystem::Init(const OpenGLWindow & inWindow)
{
	Instance = new OpenGLInputSystem{};

 	Instance->OnKeysPressed.BindRaw(Instance, &OpenGLInputSystem::OnKeyPressedTest);

 	glfwSetKeyCallback(inWindow.GetHandle(), &KeyCallback);

}


