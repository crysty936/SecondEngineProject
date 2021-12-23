#pragma once
#include "Core/ITickableObject.h"
#include "EASTL/shared_ptr.h"

class Camera : public ITickableObject
{
public:
	Camera();
	virtual ~Camera();

	virtual void Tick(const float inDeltaT) override;

public:
	eastl::shared_ptr<class Controller> OwnedController;
};





