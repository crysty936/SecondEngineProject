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
};

class Controller
{
public:
	Controller();
	virtual ~Controller();



private:
	void OnInputReceived(KeyCode inKeyCode, InputEventType inEventType);

private:
	eastl::unordered_map<KeyCode, bool> KeyStates;
};