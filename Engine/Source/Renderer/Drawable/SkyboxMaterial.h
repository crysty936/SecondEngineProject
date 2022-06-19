#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/Material/RenderMaterial.h"

class SkyboxMaterial : public RenderMaterial
{
public:
	SkyboxMaterial();
	~SkyboxMaterial();

	virtual void SetRequiredUniforms() override;
	virtual void SetUniforms(const eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache) override;
};