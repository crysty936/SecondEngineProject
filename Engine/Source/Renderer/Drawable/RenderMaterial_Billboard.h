#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/Material/RenderMaterial.h"

class RenderMaterial_Billboard : public RenderMaterial
{
public:
	RenderMaterial_Billboard();
	~RenderMaterial_Billboard();

	virtual void SetRequiredUniforms() override;

	virtual void SetUniforms(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache) override;
};