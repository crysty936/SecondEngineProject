#pragma once
#include "Math/Transform.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"

class IGameObject
{
public:
	IGameObject() = default;
	virtual ~IGameObject() = default;

	virtual void Init() = 0;
	virtual void Tick(const float inDeltaT) = 0;

public:
	Transform Model;
	eastl::vector<eastl::shared_ptr<class IGameObject>> Children;
};