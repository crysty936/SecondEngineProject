#include "Camera/Camera.h"
#include "InputSystem/InputSystem.h"


Camera::Camera()
{
	InputSystem::Get().OnKeyInput().BindRaw(this, &Camera::OnInputReceived);
}

Camera::~Camera()
{

}

void Camera::Tick(const float inDeltaT)
{



}

void Camera::OnInputReceived(KeyCode inKeyCode, InputEventType inEventType)
{
	const bool pressed = inEventType == InputEventType::InputPress;

	KeyStates[inKeyCode] = pressed;
}
