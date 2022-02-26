#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/Drawable/SimpleShapeDrawable.h"
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
	CubeShape(const eastl::string& inTexturePath = {});
	virtual ~CubeShape();

protected:
	virtual void Draw_Private() const override;

};


class BasicShapes
{
public:
// 	static eastl::shared_ptr<class DrawableBase> CreateTriangleObject(eastl::string inTexturePath = {});
// 	static eastl::shared_ptr<class DrawableBase> CreateSquareObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class DrawableBase> CreateCubeObject(eastl::string inTexturePath = {});
};
