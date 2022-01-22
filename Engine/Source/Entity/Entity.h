#pragma once
#include "Math/Transform.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"

using EntityPtr = eastl::shared_ptr<class Entity>;
using EntityIterator = eastl::vector<eastl::shared_ptr<class Entity>>::iterator;

/**
 * Main Game Entity with Transform and Init and Tick functions
 */
class Entity
{
public:
	Entity() = default;
	virtual ~Entity() = default;

	virtual void Init() = 0;
	virtual void Tick(const float inDeltaT) = 0;

	inline void AddChild(EntityPtr inEntity) { Children.push_back(inEntity); }

public:
	Transform Model;
	eastl::vector<EntityPtr> Children;
};