#include "InputSystem/InputSystem.h"
#include "Window/OpenGLWindow.h"
#include "Core/WindowsPlatform.h"

InputSystem* InputSystem::Instance;

InputSystem::InputSystem() = default;
InputSystem::~InputSystem() = default;

void InputSystem::KeyCallback(EInputKey inKey, InputEventType inAction)
{
	InputSystem& instance = InputSystem::Get();

	instance.OnKeyInputDelegate.Invoke(inKey, inAction);
}

void InputSystem::MousePosChangedCallback(const double inNewYaw, const double inNewPitch)
{
	const float yawFloat = static_cast<float>(inNewYaw);
	const float pitchFloat = static_cast<float>(inNewPitch);

	Instance->OnMouseMovedDelegate.Invoke(yawFloat, pitchFloat);
}

void InputSystem::MouseScrollCallback(double xoffset, double yoffset)
{
	const float yFloat = static_cast<float>(yoffset);

	Instance->OnMouseScrollDelegate.Invoke(yFloat);
}

void InputSystem::Init()
{
	Instance = new InputSystem{};
	

#if WITH_GLFW
	OpenGLWindow& mainWindow = RHI->GetMainWindow();

//  	Instance->OnKeyInputDelegate.BindRaw(Instance, &InputSystem::OnKeyPressedLog);
//  	glfwSetKeyCallback(mainWindow.GetHandle(), &GLFWKeyCallback);
// 	glfwSetCursorPosCallback(mainWindow.GetHandle(), &MousePosChangedCallback);
// 	glfwSetScrollCallback(mainWindow.GetHandle(), &MouseScrollCallback);

#else


#endif

}

void InputSystem::Terminate()
{
	ASSERT(Instance);

	delete(Instance);
}

void InputSystem::PollEvents()
{

#if WITH_GLFW
	glfwPollEvents();
#else
	WindowsPlatform::PoolMessages();
#endif

}

void InputSystem::OnKeyPressedLog(EInputKey inKeyCode, InputEventType inEventType)
{
	LOG_INFO("Key input received for key %d with action: %s", static_cast<int32_t>(inKeyCode), ToString(inEventType));
}
