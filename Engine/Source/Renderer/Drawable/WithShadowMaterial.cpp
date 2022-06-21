#include "Renderer/Drawable/WithShadowMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"

WithShadowMaterial::WithShadowMaterial() = default;
WithShadowMaterial::~WithShadowMaterial() = default;

void WithShadowMaterial::SetRequiredUniforms()
{
	__super::SetRequiredUniforms();
	
	RequiredUniforms.push_back({ "lightSpaceMatrix" });
	RequiredUniforms.push_back({ "ShadowMap" });
	RequiredUniforms.push_back({ "LightPos" });
}

