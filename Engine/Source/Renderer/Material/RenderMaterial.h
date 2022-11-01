#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/RHI/Resources/UniformBufferContainer.h"
#include "EASTL/unordered_map.h"

struct UniformWithFlag
{
	UniformWithFlag(const eastl::string& inName) : UniformName{ inName } {}
	eastl::string UniformName;
	bool IsSet = false;
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
	UniformWithFlag* FindRequiredUniform(const eastl::string& inUniformName);

public:
	eastl::vector<UniformWithFlag> RequiredUniforms;
	UniformBufferContainer UBuffer;
	eastl::shared_ptr<class ShaderBase> Shader;
	eastl::vector<eastl::shared_ptr<class OpenGLTexture>> Textures;
};