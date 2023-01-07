#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/Model/3D/Model3D.h"
#include "../../RenderCommand.h"

class BasicShapesHelpers
{
public:
	// 	static eastl::shared_ptr<class IDrawable> CreateTriangleObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class SquareShape> CreateSquareObject(eastl::string inTexturePath = {});
	static eastl::shared_ptr<class CubeShape> CreateCubeObject();
};

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

class MirrorQuad : public DrawableObject, public IDrawableContainer
{
public:
	MirrorQuad();
	virtual ~MirrorQuad();

	void CreateProxy() override;
};

class FullScreenQuad : public DrawableObject, public IDrawableContainer
{
public:
	FullScreenQuad(eastl::shared_ptr<RHITexture2D>& inTexture);
	virtual ~FullScreenQuad();

	void CreateProxy() override;
	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	eastl::shared_ptr<RHITexture2D> MainTexture;
	RenderCommand QuadCommand;
};
