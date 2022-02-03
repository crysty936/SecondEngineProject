#pragma once
#include "EASTL/unordered_map.h"
#include "InputSystem/GLFWInput/WindowsInputKeys.h"
#include "InputSystem/InputEventType.h"
#include "EventSystem/EventSystem.h"

using KeyActionDelegate = Delegate<void>;
using MouseMovedDelegate = Delegate<void, float, float>;

struct OnKeyAction
{
	KeyActionDelegate Del;
	KeyCode RequiredKey;
	bool Once = false;
};

class Controller
{
public:
	Controller();
	virtual ~Controller();

	void ExecuteCallbacks();
	inline void AddListener(OnKeyAction& inKeyAction) { KeyListeners.push_back(inKeyAction); }
	inline void AddMouseListener(MouseMovedDelegate inDelegate) { MouseListeners.push_back(inDelegate); }

private:
	void OnKeyInputReceived(KeyCode inKeyCode, InputEventType inEventType);
	void OnMouseInputReceived(const float inNewYaw, const float inNewPitch);

private:
	eastl::unordered_map<KeyCode, InputEventType> KeyStates;
	eastl::vector<OnKeyAction> KeyListeners;
	eastl::vector<MouseMovedDelegate> MouseListeners;
	bool bMouseMoved;
	float NewPitch;
	float NewYaw;
};