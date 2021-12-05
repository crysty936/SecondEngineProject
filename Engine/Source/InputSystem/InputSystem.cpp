#include "InputSystem/InputSystem.h"
#include "Window/OpenGLWindow.h"

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

	instance.DelegateKeyInput.Invoke(code, actionType);
}

void InputSystem::Init(const OpenGLWindow & inWindow)
{
	Instance = new InputSystem{};
	         
 	glfwSetKeyCallback(inWindow.GetHandle(), &GLFWKeyCallback);

 	Instance->DelegateKeyInput.BindRaw(Instance, &InputSystem::OnKeyPressedLog);
}

void InputSystem::OnKeyPressedLog(KeyCode inKeyCode, InputEventType inEventType)
{
    

	Logger::Get().Print("Key input received for key %d with action: %s", static_cast<int32_t>(inKeyCode), ToString(inEventType));
}