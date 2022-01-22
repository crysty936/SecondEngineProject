#pragma once
#include "Entity/Entity.h"
#include "EASTL/shared_ptr.h"

class CeilingAndFloor : public Entity
{
public:
	CeilingAndFloor();
	virtual ~CeilingAndFloor();

	virtual void Init() override;
	virtual void Tick(const float inDeltaT) override;

	static float GetFloorY();
	static float GetCeilingY();

private:
	eastl::shared_ptr<class Entity> Ceiling;
	eastl::shared_ptr<class Entity> Floor;



};