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
	for (BufferWithRequirements& buffer : UBuffers)
	{
		for (UniformWithFlag& u : buffer.RequiredUniforms)
		{
			u.IsSet = false;
		}
	}

}

void RenderMaterial::SetRequiredUniforms()
{
	// !! These have to be in the same order as they are in the shader
	eastl::vector<UniformWithFlag> defaultUniforms = {
		{"projection"},
		{"view"},
		{"model"}
		};

	UBuffers.push_back({ defaultUniforms, ConstantBufferType::Vertex });
}

void RenderMaterial::SetUniforms(eastl::unordered_map<eastl::string, SelfRegisteringUniform>& inUniformsCache)
{
	using uniformsIterator = const eastl::unordered_map<eastl::string, SelfRegisteringUniform>::const_iterator;

	for (int32_t i = 0; i < UBuffers.size(); ++i)
	{
		BufferWithRequirements& buffer = UBuffers[i];
		buffer.BufferContainer.Clear();

		// Register all required uniforms
		for (UniformWithFlag& requiredUniform : buffer.RequiredUniforms)
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
			selfRegisteringUniform.Register(buffer.BufferContainer);
			requiredUniform.IsSet = true;
		}

		buffer.BufferContainer.UpdateData(buffer.BufferType, i);
	}
}

void RenderMaterial::BindBuffers()
{
	for (BufferWithRequirements& buffer : UBuffers)
	{
		buffer.BufferContainer.Bind();
	}
}

void RenderMaterial::UnbindBuffers()
{
	for (BufferWithRequirements& buffer : UBuffers)
	{
		buffer.BufferContainer.Unbind();
	}
}

UniformWithFlag* RenderMaterial::FindRequiredUniform(const eastl::string& inUniformName)
{
// 	auto iterRequiredUniform = eastl::find_if(RequiredUniforms.begin(), RequiredUniforms.end(), [&](const UniformWithFlag& arg) {
// 		return arg.UniformName == inUniformName; });
// 
// 	if (iterRequiredUniform == RequiredUniforms.end())
// 	{
// 		return nullptr;
// 	}

	//return &*iterRequiredUniform;

	return nullptr;
}
