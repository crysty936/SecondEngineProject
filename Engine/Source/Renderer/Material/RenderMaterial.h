#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

class RenderMaterial
{
public:
	RenderMaterial();
	~RenderMaterial();

	void SetUniforms(const eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache);

public:
	eastl::vector<eastl::string> RequiredUniforms;
	OpenGLShader Shader;
	eastl::vector<OpenGLTexture> Textures;
};