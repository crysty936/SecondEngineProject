#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/Material/RenderMaterial.h"

class RenderMaterial_EditorSphere : public RenderMaterial
{
public:
	RenderMaterial_EditorSphere();
	~RenderMaterial_EditorSphere();

	virtual void SetRequiredUniforms() override;
	virtual void SetUniformsValue(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache, const EShaderType inShaderTypes = Sh_Universal) override;
};