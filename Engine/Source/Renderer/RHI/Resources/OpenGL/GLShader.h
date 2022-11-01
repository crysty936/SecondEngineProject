#pragma once
#include "Renderer/RHI/Resources/ShaderBase.h"
#include <stdint.h>

class GLShader : public ShaderBase
{
public:
	GLShader(const uint32_t inGLHandle);
	virtual void Bind() override;
	virtual void Unbind() override;

public:
	uint32_t Handle;
};