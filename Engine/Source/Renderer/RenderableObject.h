#pragma once
#include "Core/ITickableObject.h"


class RenderableObject : public ITickableObject
{
public:
	RenderableObject();
	virtual ~RenderableObject();

	virtual void Tick(const float inDeltaT) override;




};