#pragma once
#include "EASTL/unordered_map.h"
#include "InputSystem/GLFWInput/WindowsInputKeys.h"
#include "InputSystem/InputEventType.h"

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