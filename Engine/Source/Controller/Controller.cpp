#include "Controller.h"
#include "InputSystem/GLFWInput/InputSystem.h"
#include "Core/EngineCore.h"

Controller::Controller()
{
	InputSystem::Get().OnKeyInput().BindRaw(this, &Controller::OnInputReceived);
}

Controller::~Controller()
{

}

void Controller::OnInputReceived(KeyCode inKeyCode, InputEventType inEventType)
{
	if (inKeyCode == KeyCode::Escape)
	{
		StopEngineRunning();
	}

	const bool pressed = inEventType == InputEventType::InputPress;

	KeyStates[inKeyCode] = pressed;
}

