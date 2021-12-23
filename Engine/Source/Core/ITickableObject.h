#pragma once

struct ITickableObject
{
	virtual void Tick(const float inDeltaT) = 0;
};