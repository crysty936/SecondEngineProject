#pragma once
#include "Core/IGameObject.h"
#include "EASTL/shared_ptr.h"

class Camera : public IGameObject
{
public:
	Camera();
	virtual ~Camera();

	virtual void Init() override;
	virtual void Tick(const float inDeltaT) override;

	glm::mat4 GetLookAt();
};





