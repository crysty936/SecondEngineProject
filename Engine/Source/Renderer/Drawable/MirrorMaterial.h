#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/Material/RenderMaterial.h"

class MirrorMaterial : public RenderMaterial
{
public:
	MirrorMaterial();
	~MirrorMaterial();

	void SetUniforms(const eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache) override;
};