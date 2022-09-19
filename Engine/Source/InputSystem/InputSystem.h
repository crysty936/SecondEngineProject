#pragma once
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "Core/EngineUtils.h"
#include "InputSystem/InputKeys.h"
#include "InputSystem/InputEventType.h"
#include "Entity/Entity.h"
#include "CursorMode.h"
#include "Core/WindowsPlatform.h"

using KeyDelegate = MulticastDelegate<EInputKey, InputEventType>;
using MousePosDelegate = MulticastDelegate<float, float>;
using MouseScrollDelegate = MulticastDelegate<float>;

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
	inline MouseScrollDelegate& OnMouseScroll() { return OnMouseScrollDelegate; }

	void SetCursorMode(void* inWindowHandle, const ECursorMode inMode);

	void RegisterCallbacksGLFW(class OpenGLWindow& inWindow);

	bool OngoingFrameAction = false;
	bool CursorsTracked = false;
	ECursorMode CurrentCursorMode = ECursorMode::Enabled;
	glm::vec<2, int> CurrentCursorPos = {};
	glm::vec<2, int> LastCursorPos = {};

private:
	void OnKeyPressedLog(EInputKey inKeyCode, InputEventType inEventType);
	static void KeyCallback(EInputKey inKey, InputEventType inAction);
	static void GLFWKeyCallback(GLFWwindow*, int32_t inKeycode, int32_t inScanCode, int32_t inAction, int32_t inMods);
	static void MousePosChangedCallback(const double inNewYaw, const double inNewPitch);
	static void MousePosChangedCallbackOpenGL(GLFWwindow*, const double inNewYaw, const double inNewPitch);
	static void MouseScrollCallback(double xoffset, double yoffset);

private:
	static InputSystem* Instance;
	static uint16_t KeyCodes[512];

private:
	KeyDelegate OnKeyInputDelegate;
	MousePosDelegate OnMouseMovedDelegate;
	MouseScrollDelegate OnMouseScrollDelegate;

	friend class WindowsPlatform::InputForwarder;
};
