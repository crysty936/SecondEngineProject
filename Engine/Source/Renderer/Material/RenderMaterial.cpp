#include "RenderMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"

RenderMaterial::RenderMaterial() = default;
RenderMaterial::~RenderMaterial() = default;

void RenderMaterial::Init()
{
	SetRequiredUniforms();
}

void RenderMaterial::ResetUniforms()
{
	for (UniformWithFlag& u : RequiredUniforms)
	{
		u.IsSet = false;
	}
}

void RenderMaterial::SetRequiredUniforms()
{
	// Default uniforms
	RequiredUniforms = {
		{"projection"},
		{"view"},
		{"model"}
		};
}

void RenderMaterial::SetUniforms(const eastl::unordered_map<eastl::string, SelfRegisteringUniform>& inUniformsCache)
{
	using uniformsIterator = const eastl::unordered_map<eastl::string, SelfRegisteringUniform>::const_iterator;
	// Register all required uniforms
	for (UniformWithFlag& requiredUniform : RequiredUniforms)
	{
		if (requiredUniform.IsSet)
		{
			continue;
		}

		const uniformsIterator& iter = inUniformsCache.find(requiredUniform.UniformName);

		if (iter == inUniformsCache.end())
		{
			continue;
		}

		const SelfRegisteringUniform& selfRegisteringUniform = (*iter).second;
		selfRegisteringUniform.Register(requiredUniform.UniformName, Shader);
		requiredUniform.IsSet = true;
	}
}

UniformWithFlag* RenderMaterial::FindRequiredUniform(const eastl::string& inUniformName)
{
	auto iterRequiredUniform = eastl::find_if(RequiredUniforms.begin(), RequiredUniforms.end(), [&](const UniformWithFlag& arg) {
		return arg.UniformName == inUniformName; });

	if (iterRequiredUniform == RequiredUniforms.end())
	{
		return nullptr;
	}

	return &*iterRequiredUniform;
}
