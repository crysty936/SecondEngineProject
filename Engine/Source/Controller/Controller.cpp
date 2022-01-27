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

void Controller::ExecuteCallbacks()
{
	for (const OnKeyAction& actionListener : Listeners)
	{
		if (KeyStates[actionListener.RequiredKey] == InputEventType::InputPress || KeyStates[actionListener.RequiredKey]== InputEventType::InputRepeat)
		{
			if (actionListener.Once)
			{
				if (KeyStates[actionListener.RequiredKey] == InputEventType::InputPress)
				{
					// Declare it repeat until it's released
					KeyStates[actionListener.RequiredKey] = InputEventType::InputRepeat;

					actionListener.Del.Execute();
				}

				return;
			}

			actionListener.Del.Execute();
		}
	}
}

void Controller::OnInputReceived(KeyCode inKeyCode, InputEventType inEventType)
{
	if (inKeyCode == KeyCode::Escape)
	{
		StopEngineRunning();
	}

// 	switch (inEventType)
// 	{
// 	case InputEventType::InputPress:
// 		LOG_INFO("Press detected");
// 		break;
// 
// 	case InputEventType::InputRelease:
// 		LOG_INFO("Release detected");
// 		break;
// 
// 	case InputEventType::InputRepeat:
// 		LOG_INFO("Repeat detected");
// 		break;
// 	}

	KeyStates[inKeyCode] = inEventType;
}

