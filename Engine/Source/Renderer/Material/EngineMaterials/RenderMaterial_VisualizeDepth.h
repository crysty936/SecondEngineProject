#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/Material/RenderMaterial.h"

class RenderMaterial_VisualizeDepth : public RenderMaterial
{
public:
	RenderMaterial_VisualizeDepth();
	~RenderMaterial_VisualizeDepth();

	virtual void SetRequiredUniforms() override;
};