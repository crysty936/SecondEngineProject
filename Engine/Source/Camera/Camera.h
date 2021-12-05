#pragma once
#include "EASTL/unordered_map.h"
#include "InputSystem/WindowsInputKeys.h"
#include "InputSystem/InputEventType.h"
#include "Core/ITickableObject.h"


class Camera : public ITickableObject
{
public:
	Camera();
	virtual ~Camera();

public:
	virtual void Tick(const float inDeltaT) override;

private:
	void OnInputReceived(KeyCode inKeyCode, InputEventType inEventType);

private:
	eastl::unordered_map<KeyCode, bool> KeyStates;

};





