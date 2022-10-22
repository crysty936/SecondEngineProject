#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/Model/3D/Model3D.h"

class TriangleShape : public DrawableObject, public IDrawableContainer
{
public:
	TriangleShape();
	virtual ~TriangleShape();

	void CreateProxy() override;
};

 class SquareShape : public DrawableObject, public IDrawableContainer
 {
 public:
 	SquareShape();
 	virtual ~SquareShape();

	void CreateProxy() override;
 };

class CubeShape : public Model3D
{
public:
	CubeShape();
	virtual ~CubeShape();

	virtual void CreateProxy() override;
};

class BasicShapes
{
public:
// 	static eastl::shared_ptr<class IDrawable> CreateTriangleObject(eastl::string inTexturePath = {});
 	static eastl::shared_ptr<class SquareShape> CreateSquareObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class CubeShape> CreateCubeObject();
};

class Skybox : public Model3D
{
public:
	Skybox();
	virtual ~Skybox();

	virtual void CreateProxy() override;
};

class LightSource : public Model3D
{
public:
	LightSource();
	virtual ~LightSource();

	virtual void CreateProxy() override;
};