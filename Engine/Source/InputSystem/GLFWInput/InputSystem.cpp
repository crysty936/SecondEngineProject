#include "InputSystem/GLFWInput/InputSystem.h"
#include "Window/OpenGLWindow.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"

InputSystem* InputSystem::Instance;

InputSystem::InputSystem() = default;
InputSystem::~InputSystem() = default;

void InputSystem::GLFWKeyCallback(GLFWwindow*, int32_t inKeycode, int32_t inScanCode, int32_t inAction, int32_t inMods)
{
	InputSystem& instance = InputSystem::Get();

	KeyCode code = static_cast<KeyCode>(inKeycode);

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

void InputSystem::MousePosChangedCallback(GLFWwindow*, const double inNewYaw, const double inNewPitch)
{
	const float yawFloat = static_cast<float>(inNewYaw);
	const float pitchFloat = static_cast<float>(inNewPitch);

	Instance->OnMouseMovedDelegate.Invoke(yawFloat, pitchFloat);
}

void InputSystem::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	const float yFloat = static_cast<float>(yoffset);

	Instance->OnMouseScrollDelegate.Invoke(yFloat);
}

void InputSystem::Init()
{
	Instance = new InputSystem{};
	
	OpenGLWindow& mainWindow = RHI->GetMainWindow();

 	glfwSetKeyCallback(mainWindow.GetHandle(), &GLFWKeyCallback);

 	Instance->OnKeyInputDelegate.BindRaw(Instance, &InputSystem::OnKeyPressedLog);

	glfwSetCursorPosCallback(mainWindow.GetHandle(), &MousePosChangedCallback);

	glfwSetScrollCallback(mainWindow.GetHandle(), &MouseScrollCallback);
}

void InputSystem::Terminate()
{
	ASSERT_MSG(Instance);

	delete(Instance);
}

void InputSystem::PollEvents()
{
	glfwPollEvents();
}

void InputSystem::OnKeyPressedLog(KeyCode inKeyCode, InputEventType inEventType)
{
	LOG_INFO("Key input received for key %d with action: %s", static_cast<int32_t>(inKeyCode), ToString(inEventType));
}