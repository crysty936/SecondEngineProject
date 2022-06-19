#include "Renderer/Drawable/SkyboxMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"

SkyboxMaterial::SkyboxMaterial() = default;
SkyboxMaterial::~SkyboxMaterial() = default;

void SkyboxMaterial::SetRequiredUniforms()
{
	RequiredUniforms = {
		{"projection"},
		{"view"},
		};
}

void SkyboxMaterial::SetUniforms(const eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache)
{
	const eastl::string viewName = "view";
	const auto& iter = inUniformsCache.find(viewName);

	if (iter == inUniformsCache.end())
	{
		ASSERT(false);

		return;
	}

	UniformWithFlag* requiredUniform = FindRequiredUniform(viewName);

 	if (!requiredUniform)
 	{
 		return;
 	}

	glm::mat4 view = (*iter).second.Value.Value4fv;
	// Remove translation by taking the upper 3x3 of the matrix then casting to 4x4
	view = glm::mat4(glm::mat3(view));

	Shader.SetUniformValue4fv(viewName, view);
	requiredUniform->IsSet = true;
	
	__super::SetUniforms(inUniformsCache);
}

