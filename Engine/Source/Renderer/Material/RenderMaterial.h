#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

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
	virtual void SetUniforms(const eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache);
	
	UniformWithFlag* FindRequiredUniform(const eastl::string& inUniformName);

public:
	eastl::vector<UniformWithFlag> RequiredUniforms;
	OpenGLShader Shader;
	eastl::vector<OpenGLTexture> Textures;
};