#include "InputSystem/InputSystem.h"
#include "Core/WindowsPlatform.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Window/WindowsWindow.h"
#include "Core/EngineCore.h"
#include "Window/OpenGLWindow.h"

InputSystem* InputSystem::Instance;

InputSystem::InputSystem() = default;
InputSystem::~InputSystem() = default;

void InputSystem::KeyCallback(EInputKey inKey, InputEventType inAction)
{
	InputSystem& instance = InputSystem::Get();

	instance.OnKeyInputDelegate.Invoke(inKey, inAction);
}

void InputSystem::GLFWKeyCallback(GLFWwindow*, int32_t inKeycode, int32_t inScanCode, int32_t inAction, int32_t inMods)
{
	InputSystem& instance = InputSystem::Get();

	const EInputKey code = static_cast<EInputKey>(inKeycode);

	InputEventType actionType = InputEventType::None;

	switch (inAction)
	{
	case GLFW_PRESS:
		actionType = InputEventType::InputPress;
		break;
	case GLFW_RELEASE:
		actionType = InputEventType::InputRelease;
		break;
	case GLFW_REPEAT:
		actionType = InputEventType::InputRepeat;
		break;
	default:
		break;
	}

	instance.OnKeyInputDelegate.Invoke(code, actionType);
}

void InputSystem::MousePosChangedCallback(const double inNewYaw, const double inNewPitch)
{
	const float yawFloat = static_cast<float>(inNewYaw);
	const float pitchFloat = static_cast<float>(inNewPitch);

	Instance->OnMouseMovedDelegate.Invoke(yawFloat, pitchFloat);
}

void InputSystem::MousePosChangedCallbackOpenGL(GLFWwindow*, double inNewYaw, double inNewPitch)
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

#if WITH_GLFW
void InputSystem::RegisterCallbacksGLFW(OpenGLWindow& inWindow)
{
	//  	Instance->OnKeyInputDelegate.BindRaw(Instance, &InputSystem::OnKeyPressedLog);
	glfwSetKeyCallback(inWindow.GetHandle(), &GLFWKeyCallback);
	glfwSetCursorPosCallback(inWindow.GetHandle(), &MousePosChangedCallbackOpenGL);
	// 	glfwSetScrollCallback(mainWindow.GetHandle(), &MouseScrollCallback);

}
#endif

void InputSystem::Init()
{
	Instance = new InputSystem{};
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

void InputSystem::SetCursorMode(void* inWindowHandle, const ECursorMode inMode)
{
	CurrentCursorMode = inMode;
	WindowsPlatform::SetCursorMode(inWindowHandle, inMode);
}

void InputSystem::OnKeyPressedLog(EInputKey inKeyCode, InputEventType inEventType)
{
	LOG_INFO("Key input received for key %d with action: %s", static_cast<int32_t>(inKeyCode), ToString(inEventType));
}
