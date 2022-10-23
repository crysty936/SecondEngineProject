#pragma once
#include "Renderer/RHI/Resources/ShaderBase.h"
#include <stdint.h>

class GLShader : public ShaderBase
{
public:
	uint32_t Handle;
};