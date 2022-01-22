#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/OpenGL/DrawableObject.h"

class SquareShape : public DrawableObject
{
public:
	SquareShape(const eastl::string& inTexturePath = {});
	virtual ~SquareShape();


	virtual void Tick(const float inDeltaT) override;
};

class BasicShapes
{
public:
	static eastl::shared_ptr<class IGameObject> CreateTriangleObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class IGameObject> CreateSquareObject(eastl::string inTexturePath = {});
};
