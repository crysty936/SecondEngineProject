#include "Controller.h"
#include "InputSystem/GLFWInput/InputSystem.h"
#include "Core/EngineCore.h"

Controller::Controller()
	: KeyStates()
	, KeyListeners()
	, MouseListeners()
	, bMouseMoved{ false }
	, NewPitch{0.0f}
	, NewYaw{0.0f}
{
	InputSystem::Get().OnKeyInput().BindRaw(this, &Controller::OnKeyInputReceived);
	InputSystem::Get().OnMouseMoved().BindRaw(this, &Controller::OnMouseInputReceived);
}

Controller::~Controller() = default;

void Controller::ExecuteCallbacks()
{
	if (bMouseMoved)
	{
		bMouseMoved = false;

		for (const MouseMovedDelegate& listener : MouseListeners)
		{
			listener.Execute(NewYaw, NewPitch);
		}
	}

	for (const OnKeyAction& listener : KeyListeners)
	{
		if (KeyStates[listener.RequiredKey] == InputEventType::InputPress || KeyStates[listener.RequiredKey] == InputEventType::InputRepeat)
		{
			if (listener.Once)
			{
				if (KeyStates[listener.RequiredKey] == InputEventType::InputPress)
				{
					// Declare it repeat until it's released
					KeyStates[listener.RequiredKey] = InputEventType::InputRepeat;

					listener.Del.Execute();
				}

				return;
			}

			listener.Del.Execute();
		}
	}
}

void Controller::OnKeyInputReceived(KeyCode inKeyCode, InputEventType inEventType)
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

void Controller::OnMouseInputReceived(const float inNewYaw, const float inNewPitch)
{
	bMouseMoved = true;
	NewYaw = inNewYaw;
	NewPitch = inNewPitch;
}

