#pragma once
#include "EASTL/array.h"
#include "glm/detail/type_vec3.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "RenderCommand.h"
#include "Drawable/Drawable.h"
#include "Model/3D/Model3D.h"
#include "LightTypes.h"

namespace RenderUtils
{
	eastl::array<glm::vec3, 8> GenerateSpaceCorners(const glm::mat4& SpaceToProjectionSpace, const float MinZ = 0.f, const float MaxZ = 1.f);
	glm::vec3 GetProjectionCenter(const glm::mat4& inProj);
}

class Skybox : public Model3D
{
public:
	Skybox(const eastl::string& inName);
	virtual ~Skybox();

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
	FullScreenQuad(const eastl::string& inName);
	virtual ~FullScreenQuad();

	void CreateProxy() override;
	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

class GBufferVisualizeQuad : public DrawableObject, public IDrawableContainer
{
public:
	GBufferVisualizeQuad(const eastl::string& inName);
	virtual ~GBufferVisualizeQuad();

	void CreateProxy() override;
	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

class ToneMapQuad : public DrawableObject
{
public:
	ToneMapQuad(const eastl::string& inName);
	~ToneMapQuad() = default;

	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

class ExtractBrightAreasQuad : public DrawableObject
{
public:
	ExtractBrightAreasQuad(const eastl::string& inName);
	~ExtractBrightAreasQuad() = default;

	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

class GaussianBlurQuad : public DrawableObject
{
public:
	GaussianBlurQuad(const eastl::string& inName);
	~GaussianBlurQuad() = default;

	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

class BloomMergeQuad : public DrawableObject
{
public:
	BloomMergeQuad(const eastl::string& inName);
	~BloomMergeQuad() = default;

	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

class VisualizeDepthQuad : public DrawableObject
{
public:
	VisualizeDepthQuad(const eastl::string& inName);
	~VisualizeDepthQuad() = default;

	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

class DefaultLightingModelQuad : public DrawableObject
{
public:
	DefaultLightingModelQuad(const eastl::string& inName);
	~DefaultLightingModelQuad() = default;

	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

class DefaultPBRLightingModelQuad : public DrawableObject
{
public:
	DefaultPBRLightingModelQuad(const eastl::string& inName);
	~DefaultPBRLightingModelQuad() = default;

	void CreateCommand();
	inline const RenderCommand& GetCommand() { return QuadCommand; };

private:
	RenderCommand QuadCommand;
};

