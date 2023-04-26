#pragma once
#include "EASTL/array.h"
#include "glm/detail/type_vec3.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "RenderCommand.h"
#include "Drawable/Drawable.h"

namespace RenderUtils
{
	eastl::array<glm::vec3, 8> GenerateSpaceCorners(const glm::mat4& SpaceToProjectionSpace, const float MinZ = 0.f, const float MaxZ = 1.f);
	glm::vec3 GetProjectionCenter(const glm::mat4& inProj);
}

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
