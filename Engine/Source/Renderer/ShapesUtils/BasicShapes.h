#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/OpenGL/DrawableObject.h"

class SquareShape : public DrawableObject
{
public:
	SquareShape();
	virtual ~SquareShape();


	virtual void Tick(const float inDeltaT) override;
};

class BasicShapes
{
public:
	static eastl::shared_ptr<class IGameObject> CreateTriangleObject();
	static eastl::shared_ptr<class IGameObject> CreateSquareObject();
};
