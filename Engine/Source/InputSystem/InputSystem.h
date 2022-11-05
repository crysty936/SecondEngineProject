#pragma once
#include "EventSystem/EventSystem.h"
#include "Core/EngineUtils.h"
#include "InputSystem/InputKeys.h"
#include "InputSystem/InputType.h"
#include "Entity/Entity.h"
#include "CursorMode.h"
#include "Core/WindowsPlatform.h"
#include "Core/EngineCore.h"

using KeyDelegate = MulticastDelegate<EInputKey, EInputType>;
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

	bool OngoingFrameAction = false;
	bool CursorsTracked = false;
	ECursorMode CurrentCursorMode = ECursorMode::Enabled;
	glm::vec<2, int> CurrentCursorPos = {};
	glm::vec<2, int> LastCursorPos = {};

private:
	void OnKeyPressedLog(EInputKey inKeyCode, EInputType inEventType);
	static void KeyCallback(EInputKey inKey, EInputType inAction);
	static void MousePosChangedCallback(const double inNewYaw, const double inNewPitch);
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
