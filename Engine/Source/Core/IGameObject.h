#pragma once
#include "Math/Transform.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"

struct IGameObject
{
	Transform Model;

	virtual void Init() = 0;
	virtual void Tick(const float inDeltaT) = 0;

	eastl::vector<eastl::shared_ptr<struct IGameObject>> Children;
};