#pragma once
#include "EASTL/shared_ptr.h"
#include "Renderer/Model/3D/Model3D.h"
#include "../../RenderCommand.h"
#include "../../LightTypes.h"

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

class Skybox : public Model3D
{
public:
	Skybox(const eastl::string& inName);
	virtual ~Skybox();

	virtual void CreateProxy() override;
};

class LightSource : public Model3D
{
public:
	LightSource(const eastl::string& inName, const ELightType inType);
	virtual ~LightSource();

	ELightType Type = ELightType::Directional;

	virtual void CreateProxy() override;
};

class MirrorQuad : public DrawableObject, public IDrawableContainer
{
public:
	MirrorQuad(const eastl::string& inName);
	virtual ~MirrorQuad();

	void CreateProxy() override;
};

class FullScreenQuad : public DrawableObject, public IDrawableContainer
{
public:
	FullScreenQuad(eastl::shared_ptr<RHITexture2D>& inTexture, const eastl::string& inName);
	virtual ~FullScreenQuad();

	void CreateProxy() override;
	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};
