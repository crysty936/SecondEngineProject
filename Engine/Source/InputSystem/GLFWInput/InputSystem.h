#pragma once
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "Core/EngineUtils.h"
#include "InputSystem/GLFWInput/WindowsInputKeys.h"
#include "InputSystem/InputEventType.h"
#include "Entity/Entity.h"

using KeyDelegate = MulticastDelegate<KeyCode, InputEventType>;
using MousePosDelegate = MulticastDelegate<float, float>;

class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	static void Init();
	static void Terminate();
	void PollEvents();

public:
	static inline InputSystem& Get() { ASSERT(Instance); return *Instance; }

	/** Callbacks for others to tie into */
	inline KeyDelegate& OnKeyInput() { return OnKeyInputDelegate; }
	inline MousePosDelegate& OnMouseMoved() { return OnMouseMovedDelegate; }

private:
	void OnKeyPressedLog(KeyCode inKeyCode, InputEventType inEventType);
	static void GLFWKeyCallback(GLFWwindow*, int32_t inKeycode, int32_t inScanCode, int32_t inAction, int32_t inMods);
	static void MousePosChangedCallback(GLFWwindow*, const double inNewYaw, const double inNewPitch);

private:
	static InputSystem* Instance;

private:
	KeyDelegate OnKeyInputDelegate;
	MousePosDelegate OnMouseMovedDelegate;
};
