#pragma once
#include "Entity/Entity.h"
#include "EASTL/shared_ptr.h"

class Camera : public Entity
{
public:
	Camera();
	virtual ~Camera();

	virtual void Init() override;
	virtual void Tick(const float inDeltaT) override;

	glm::mat4 GetLookAt();
};


// TODO: Implement raycasts

// Deprojecting screen to world, used to convert mouse position to world space to be able to do raycasts - in Unreal in method DeprojectScreenToWorld
// used by GetHitResultUnderCursorByHand to get the location of the raycast

