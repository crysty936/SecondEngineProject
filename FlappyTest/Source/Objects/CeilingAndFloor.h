#pragma once
#include "Core/IGameObject.h"
#include "EASTL/shared_ptr.h"

class CeilingAndFloor : public IGameObject
{
public:
	CeilingAndFloor();
	virtual ~CeilingAndFloor();

	virtual void Init() override;
	virtual void Tick(const float inDeltaT) override;

	static float GetFloorY();
	static float GetCeilingY();

private:
	eastl::shared_ptr<class IGameObject> Ceiling;
	eastl::shared_ptr<class IGameObject> Floor;



};