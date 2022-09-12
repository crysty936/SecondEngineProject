#pragma once
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "Core/EngineUtils.h"
#include "InputSystem/WindowsInputKeys.h"
#include "InputSystem/InputEventType.h"
#include "Entity/Entity.h"

using KeyDelegate = MulticastDelegate<KeyCode, InputEventType>;
using MousePosDelegate = MulticastDelegate<float, float>;
using MouseScrollDelegate = MulticastDelegate<float>;

class OpenGLInputSystem
{
private:
	OpenGLInputSystem();
	~OpenGLInputSystem();

public:
	static void Init();
	static void Terminate();
	void PollEvents();

public:
	static inline OpenGLInputSystem& Get() { ASSERT(Instance); return *Instance; }

	/** Callbacks for others to tie into */
	inline KeyDelegate& OnKeyInput() { return OnKeyInputDelegate; }
	inline MousePosDelegate& OnMouseMoved() { return OnMouseMovedDelegate; }
	inline MouseScrollDelegate& OnMouseScroll() { return OnMouseScrollDelegate; }

private:
	void OnKeyPressedLog(KeyCode inKeyCode, InputEventType inEventType);
	static void GLFWKeyCallback(GLFWwindow*, int32_t inKeycode, int32_t inScanCode, int32_t inAction, int32_t inMods);
	static void MousePosChangedCallback(GLFWwindow*, const double inNewYaw, const double inNewPitch);
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	static OpenGLInputSystem* Instance;

private:
	KeyDelegate OnKeyInputDelegate;
	MousePosDelegate OnMouseMovedDelegate;
	MouseScrollDelegate OnMouseScrollDelegate;
};
