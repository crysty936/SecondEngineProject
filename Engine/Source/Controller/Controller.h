#pragma once

#include "InputSystem/GLFWInput/OpenGLInputSystem.h"
#include "EASTL/unordered_map.h"
#include "InputSystem/WindowsInputKeys.h"
#include "InputSystem/InputEventType.h"
#include "EventSystem/EventSystem.h"

using KeyActionDelegate = Delegate<void>;

struct OnKeyAction
{
	KeyActionDelegate Del{};
	KeyCode RequiredKey{};
	bool Once{ false };
};

class Controller
{
public:
	Controller();
	virtual ~Controller();

	void ExecuteCallbacks();
	inline void AddListener(OnKeyAction& inKeyAction) { KeyListeners.push_back(inKeyAction); }
	inline MouseScrollDelegate& OnMouseScroll() { return OnMouseScrollDelegate; }
	inline MousePosDelegate& OnMouseMoved() { return OnMouseMovedDelegate; }

private:
	void OnKeyInputReceived(KeyCode inKeyCode, InputEventType inEventType);
	void OnMouseMoveInputReceived(const float inNewYaw, const float inNewPitch);
	void OnMouseScrollInputReceived(const float inNewY);

private:
	eastl::unordered_map<KeyCode, InputEventType> KeyStates{};
	eastl::vector<OnKeyAction> KeyListeners{};
	MousePosDelegate OnMouseMovedDelegate{};
	MouseScrollDelegate OnMouseScrollDelegate{};
	bool bMouseMoved{false};
	bool bMouseScrolled{false};
	float NewPitch{0.0f};
	float NewYaw{0.0f};
	float NewMouseScrollOffset{ 0.0f };
};