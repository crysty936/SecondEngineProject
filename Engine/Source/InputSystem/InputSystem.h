#pragma once
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "Core/EngineUtils.h"
#include "InputSystem/WindowsInputKeys.h"
#include "InputSystem/InputEventType.h"

using KeyDelegate = MulticastDelegate<KeyCode, InputEventType>;

class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	static void Init(const class OpenGLWindow& inWindow);
	static inline InputSystem& Get() { ASSERT(Instance); return *Instance; }

public:
	// Callbacks section for others to tie into
	inline KeyDelegate& OnKeyInput() { return DelegateKeyInput; }

private:
	void OnKeyPressedLog(KeyCode inKeyCode, InputEventType inEventType);
	static void GLFWKeyCallback(GLFWwindow*, int32_t inKeycode, int32_t inScanCode, int32_t inAction, int32_t inMods);

private:
	static InputSystem* Instance;

private:
	KeyDelegate DelegateKeyInput;
};
