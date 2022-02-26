#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "Renderer/OpenGL/OpenGLShader.h"

struct Material
{
	eastl::vector<eastl::string> RequiredUniforms = {
		"projection",
		"view",
		"model"
	};
	OpenGLShader Shader;
};