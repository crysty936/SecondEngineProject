#include "InputSystem/GLFWInput/OpenGLInputSystem.h"
#include "Window/OpenGLWindow.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"

OpenGLInputSystem* OpenGLInputSystem::Instance;

OpenGLInputSystem::OpenGLInputSystem() = default;
OpenGLInputSystem::~OpenGLInputSystem() = default;

void OpenGLInputSystem::GLFWKeyCallback(GLFWwindow*, int32_t inKeycode, int32_t inScanCode, int32_t inAction, int32_t inMods)
{
	OpenGLInputSystem& instance = OpenGLInputSystem::Get();

	const KeyCode code = static_cast<KeyCode>(inKeycode);

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

void OpenGLInputSystem::MousePosChangedCallback(GLFWwindow*, const double inNewYaw, const double inNewPitch)
{
	const float yawFloat = static_cast<float>(inNewYaw);
	const float pitchFloat = static_cast<float>(inNewPitch);

	Instance->OnMouseMovedDelegate.Invoke(yawFloat, pitchFloat);
}

void OpenGLInputSystem::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	const float yFloat = static_cast<float>(yoffset);

	Instance->OnMouseScrollDelegate.Invoke(yFloat);
}

void OpenGLInputSystem::Init()
{
	Instance = new OpenGLInputSystem{};
	
	OpenGLWindow& mainWindow = RHI->GetMainWindow();

 	glfwSetKeyCallback(mainWindow.GetHandle(), &GLFWKeyCallback);

 	Instance->OnKeyInputDelegate.BindRaw(Instance, &OpenGLInputSystem::OnKeyPressedLog);

	glfwSetCursorPosCallback(mainWindow.GetHandle(), &MousePosChangedCallback);

	glfwSetScrollCallback(mainWindow.GetHandle(), &MouseScrollCallback);
}

void OpenGLInputSystem::Terminate()
{
	ASSERT(Instance);

	delete(Instance);
}

void OpenGLInputSystem::PollEvents()
{
	glfwPollEvents();
}

void OpenGLInputSystem::OnKeyPressedLog(KeyCode inKeyCode, InputEventType inEventType)
{
	//LOG_INFO("Key input received for key %d with action: %s", static_cast<int32_t>(inKeyCode), ToString(inEventType));
}