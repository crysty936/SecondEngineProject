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
