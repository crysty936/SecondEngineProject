#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/Material/RenderMaterial.h"

class RenderMaterial_WithShadow : public RenderMaterial
{
public:
	RenderMaterial_WithShadow();
	~RenderMaterial_WithShadow();

	virtual void SetRequiredUniforms() override;
	virtual void SetUniforms(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache) override;
};