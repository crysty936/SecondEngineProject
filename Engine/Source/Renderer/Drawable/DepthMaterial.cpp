#include "Renderer/Drawable/DepthMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"

DepthMaterial::DepthMaterial() = default;
DepthMaterial::~DepthMaterial() = default;

void DepthMaterial::SetRequiredUniforms()
{
	RequiredUniforms = {
		{"model"},
		{"lightSpaceMatrix"},
		};
}

