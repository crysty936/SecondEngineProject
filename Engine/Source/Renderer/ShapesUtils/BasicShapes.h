#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/OpenGL/DrawableObject.h"

class TriangleShape : public DrawableObject
{
public:
	TriangleShape(const eastl::string& inTexturePath = {});
	virtual ~TriangleShape();

	virtual void Tick(const float inDeltaT) override;
};

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
	static eastl::shared_ptr<class Entity> CreateTriangleObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class Entity> CreateSquareObject(eastl::string inTexturePath = {});
};
