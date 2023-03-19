#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/Material/RenderMaterial.h"

class RenderMaterial_Parallax : public RenderMaterial
{
public:
	RenderMaterial_Parallax();
	~RenderMaterial_Parallax();

	virtual void SetRequiredUniforms() override;
};