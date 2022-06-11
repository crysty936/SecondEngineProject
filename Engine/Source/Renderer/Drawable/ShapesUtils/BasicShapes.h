#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/Model/3D/Model3D.h"

// class TriangleShape : public SimpleShapeDrawable
// {
// public:
// 	TriangleShape(const eastl::string& inTexturePath = {});
// 	virtual ~TriangleShape();
// };
// 
 class SquareShape : public DrawableObject, public IDrawableContainer
 {
 public:
 	SquareShape();
 	virtual ~SquareShape();

	void SetupDrawCommands() override;
 };

class CubeShape : public Model3D
{
public:
	CubeShape();
	virtual ~CubeShape();

	virtual void SetupDrawCommands() override;
};

class BasicShapes
{
public:
// 	static eastl::shared_ptr<class IDrawable> CreateTriangleObject(eastl::string inTexturePath = {});
 	static eastl::shared_ptr<class SquareShape> CreateSquareObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class CubeShape> CreateCubeObject();
};
