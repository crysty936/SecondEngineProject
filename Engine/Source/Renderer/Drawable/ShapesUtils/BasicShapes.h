#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/Model/3D/Model3D.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/LightTypes.h"

class TriangleShape : public DrawableObject, public IDrawableContainer
{
public:
	TriangleShape(const eastl::string& inName);
	virtual ~TriangleShape();

	void CreateProxy() override;
};

 class SquareShape : public DrawableObject, public IDrawableContainer
 {
 public:
 	SquareShape(const eastl::string& inName);
 	virtual ~SquareShape();

	void CreateProxy() override;
 };

class CubeShape : public Model3D
{
public:
	CubeShape(const eastl::string& inName);
	virtual ~CubeShape();

	virtual void CreateProxy() override;
};

class LightSource : public Model3D
{
public:
	LightSource(const eastl::string& inName);
	virtual ~LightSource();

	LightData LData;

	virtual void CreateProxy() override;
};


class DeferredDecal : public Model3D
{
public:
	DeferredDecal(const eastl::string& inName);
	virtual ~DeferredDecal();

	virtual void CreateProxy() override;
};
