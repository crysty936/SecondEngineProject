#pragma once
#include "Renderer/RHI/Resources/RHIShader.h"
#include <stdint.h>

class GLShader : public RHIShader
{
public:
	GLShader(const uint32_t inGLHandle);

public:
	uint32_t Handle;
};