#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/OpenGL/SimpleShapeDrawable.h"

class TriangleShape : public SimpleShapeDrawable
{
public:
	TriangleShape(const eastl::string& inTexturePath = {});
	virtual ~TriangleShape();

	virtual void Tick(const float inDeltaT) override;
};

class SquareShape : public SimpleShapeDrawable
{
public:
	SquareShape(const eastl::string& inTexturePath = {});
	virtual ~SquareShape();


	virtual void Tick(const float inDeltaT) override;
};

class CubeShape : public SimpleShapeDrawable
{
public:
	CubeShape(const eastl::string& inTexturePath = {});
	virtual ~CubeShape();

	virtual void Tick(const float inDeltaT)	override;

protected:
	virtual void Draw_Private() const override;

};


class BasicShapes
{
public:
	static eastl::shared_ptr<class Entity> CreateTriangleObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class Entity> CreateSquareObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class Entity> CreateCubeObject(eastl::string inTexturePath = {});
};
