#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/Material/RenderMaterial.h"

class WithShadowMaterial : public RenderMaterial
{
public:
	WithShadowMaterial();
	~WithShadowMaterial();

	virtual void SetRequiredUniforms() override;
};