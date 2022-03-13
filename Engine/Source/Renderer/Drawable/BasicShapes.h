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
// class SquareShape : public SimpleShapeDrawable
// {
// public:
// 	SquareShape(const eastl::string& inTexturePath = {});
// 	virtual ~SquareShape();
// };

class CubeShape : public Model3D
{
public:
	CubeShape();
	virtual ~CubeShape();

	virtual void SetupDrawCommand() override;
};

class BasicShapes
{
public:
// 	static eastl::shared_ptr<class IDrawable> CreateTriangleObject(eastl::string inTexturePath = {});
// 	static eastl::shared_ptr<class IDrawable> CreateSquareObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class CubeShape> CreateCubeObject();
};
