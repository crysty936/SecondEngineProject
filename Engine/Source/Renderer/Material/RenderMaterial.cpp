#include "RenderMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"

RenderMaterial::RenderMaterial()
{
	// Default uniforms
	RequiredUniforms = {
		"projection",
		"view",
		"model"
	};
}

RenderMaterial::~RenderMaterial() = default;

void RenderMaterial::SetUniforms(const eastl::unordered_map<eastl::string, SelfRegisteringUniform>& inUniformsCache)
{
	using uniformsIterator = const eastl::unordered_map<eastl::string, SelfRegisteringUniform>::const_iterator;
	// Register all required uniforms
	for (const eastl::string& thisUniformName : RequiredUniforms)
	{
		const uniformsIterator& iter = inUniformsCache.find(thisUniformName);

		if (iter == inUniformsCache.end())
		{
			continue;
		}

		const SelfRegisteringUniform& selfRegisteringUniform = (*iter).second;
		selfRegisteringUniform.Register(thisUniformName, Shader);
	}
}
