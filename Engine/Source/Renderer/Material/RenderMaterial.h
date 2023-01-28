#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/RHI/Resources/UniformBufferContainer.h"
#include "EASTL/unordered_map.h"
#include "Renderer/RHI/Resources/RHIUniformBuffer.h"

struct UniformWithFlag
{
	UniformWithFlag(const eastl::string& inName) : UniformName{ inName } {}
	eastl::string UniformName;
	bool IsSet = false;
};

struct BufferWithRequirements
{
	BufferWithRequirements(eastl::vector<UniformWithFlag>& inRequiredUniforms, ConstantBufferType inBufferType)
		: RequiredUniforms(inRequiredUniforms), BufferType(inBufferType), BufferContainer()
	{}

	eastl::vector<UniformWithFlag> RequiredUniforms;
	ConstantBufferType BufferType = ConstantBufferType::Vertex;
	UniformBufferContainer BufferContainer;
};

class RenderMaterial
{
public:
	RenderMaterial();
	~RenderMaterial();

	virtual void Init();
	void ResetUniforms();
	virtual void SetRequiredUniforms();
	virtual void SetUniforms(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache);
	void BindBuffers();
	void UnbindBuffers();
	UniformWithFlag* FindRequiredUniform(const eastl::string& inUniformName);

public:
	eastl::vector<BufferWithRequirements> UBuffers;
	eastl::shared_ptr<class RHIShader> Shader;

	eastl::vector<eastl::shared_ptr<class RHITexture2D>> OwnedTextures;
	eastl::vector<eastl::weak_ptr<class RHITexture2D>> WeakTextures;
};