#include "InputSystem/OpenGlInputSystem.h"
#include "Window/OpenGLWindow.h"

OpenGLInputSystem::OpenGLInputSystem() = default;

OpenGLInputSystem::~OpenGLInputSystem() = default;

void OpenGLInputSystem::OnKeyPressedTest(int keycode, int scanCode, int action, int mods)
{
	Logger::Get().Print("Key input received.");
}

OpenGLInputSystem* OpenGLInputSystem::Instance;

void KeyCallback(GLFWwindow*, int keycode, int scanCode, int action, int mods)
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


