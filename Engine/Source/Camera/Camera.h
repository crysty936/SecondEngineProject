#pragma once
#include "Entity/Entity.h"
#include "EASTL/shared_ptr.h"


enum class MovementDirection
{
	Forward,
	Back,
	Left,
	Right
};

class Camera : public Entity
{
public:
	Camera();
	virtual ~Camera();

	virtual void Init() override;
	virtual void Tick(const float inDeltaT) override;
	
	void Move(MovementDirection inDirection, const float inSpeed = 0.1f);
	void SetMovementDelegates(class Controller& inController);
	glm::mat4 GetLookAt();

private:
	void OnMousePosChanged(const float inNewYaw, const float inNewPitch);
	
private:
	bool FirstMouse;
	float MouseLastYaw;
	float MouseLastPitch;
	float Yaw = 0.f;
	float Pitch = 0.f;
};


// TODO: Implement raycasts
