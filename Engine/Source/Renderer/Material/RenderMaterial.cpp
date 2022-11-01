#include "RenderMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"

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
	// !! These have to be in the same order as they are in the shader
	// Default uniforms
	RequiredUniforms = {
		{"projection"},
		{"view"},
		{"model"}
		};
}

void RenderMaterial::SetUniforms(eastl::unordered_map<eastl::string, SelfRegisteringUniform>& inUniformsCache)
{
	using uniformsIterator = const eastl::unordered_map<eastl::string, SelfRegisteringUniform>::const_iterator;

	UBuffer.Clear();

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
			ENSURE_MSG(false, "Failed to find uniform with name %s in cache.");
			continue;
		}

		const SelfRegisteringUniform& selfRegisteringUniform = (*iter).second;
		selfRegisteringUniform.Register(UBuffer);
		requiredUniform.IsSet = true;
	}

	UBuffer.UpdateData();
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
