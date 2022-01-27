#pragma once
#include "EASTL/unordered_map.h"
#include "InputSystem/GLFWInput/WindowsInputKeys.h"
#include "InputSystem/InputEventType.h"
#include "EventSystem/EventSystem.h"

using KeyActionDelegate = Delegate<void>;

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
	inline void AddListener(OnKeyAction& inKeyAction) { Listeners.push_back(inKeyAction); }

private:
	void OnInputReceived(KeyCode inKeyCode, InputEventType inEventType);

private:
	eastl::unordered_map<KeyCode, InputEventType> KeyStates;
	eastl::vector<OnKeyAction> Listeners;
};