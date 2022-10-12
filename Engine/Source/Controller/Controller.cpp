#include "Controller.h"
#include "Core/EngineCore.h"

Controller::Controller()
{
	InputSystem::Get().OnKeyInput().BindRaw(this, &Controller::OnKeyInputReceived);
	InputSystem::Get().OnMouseMoved().BindRaw(this, &Controller::OnMouseMoveInputReceived);
	InputSystem::Get().OnMouseScroll().BindRaw(this, &Controller::OnMouseScrollInputReceived);
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
		if (KeyStates[listener.RequiredKey] == EInputType::InputPress || KeyStates[listener.RequiredKey] == EInputType::InputRepeat)
		{
			if (listener.Once)
			{
				if (KeyStates[listener.RequiredKey] == EInputType::InputPress)
				{
					// Declare it repeat until it's released
					KeyStates[listener.RequiredKey] = EInputType::InputRepeat;

					listener.Del.Execute();
				}

				return;
			}

			listener.Del.Execute();
		}
	}
}

void Controller::OnKeyInputReceived(EInputKey inKeyCode, EInputType inEventType)
{
 	if (inKeyCode == EInputKey::Escape)
 	{
 		StopEngine();
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

