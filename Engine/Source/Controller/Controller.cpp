#include "Controller.h"
#include "Core/EngineCore.h"

Controller::Controller()
{
	OpenGLInputSystem::Get().OnKeyInput().BindRaw(this, &Controller::OnKeyInputReceived);
	OpenGLInputSystem::Get().OnMouseMoved().BindRaw(this, &Controller::OnMouseMoveInputReceived);
	OpenGLInputSystem::Get().OnMouseScroll().BindRaw(this, &Controller::OnMouseScrollInputReceived);
}

Controller::~Controller() = default;

void Controller::ExecuteCallbacks()
{
	if (bMouseScrolled)
	{
		bMouseScrolled = false;

		OnMouseScrollDelegate.Invoke(NewMouseScrollOffset);
	}

	if (bMouseMoved)
	{
		bMouseMoved = false;

		OnMouseMovedDelegate.Invoke(NewYaw, NewPitch);
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

void Controller::OnMouseMoveInputReceived(const float inNewYaw, const float inNewPitch)
{
	bMouseMoved = true;
	NewYaw = inNewYaw;
	NewPitch = inNewPitch;
}

void Controller::OnMouseScrollInputReceived(const float inNewY)
{
	bMouseScrolled = true;
	NewMouseScrollOffset = inNewY;
}

