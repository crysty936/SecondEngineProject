#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/Material/RenderMaterial.h"

class RenderMaterial_WithLighting : public RenderMaterial
{
public:
	RenderMaterial_WithLighting();
	~RenderMaterial_WithLighting();

	virtual void SetRequiredUniforms() override;
};