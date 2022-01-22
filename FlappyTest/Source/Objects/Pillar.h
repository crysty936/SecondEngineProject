#pragma once
#include "Entity/Entity.h"

class Pillar : public Entity
{
public:
	Pillar();
	virtual ~Pillar();


	virtual void Init() override;
	virtual void Tick(const float inDeltaT) override;

private:
	EntityPtr TopPillarDrawable;
	EntityPtr BottomPillarDrawable;
};