#pragma once
#include "Entity.h"

/**
 * Empty Gameobject that is used for it's Model
 */
class TransformEntity : public Entity
{
public:
	TransformEntity();
	virtual ~TransformEntity();


	virtual void Init() override;
	virtual void Tick(const float inDeltaT) override;
};