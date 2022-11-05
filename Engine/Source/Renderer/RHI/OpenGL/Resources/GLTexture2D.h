#pragma once
#include "Renderer/RHI/Resources/RHITexture.h"

class GLTexture2D : public RHITexture2D
{
public:
	GLTexture2D(uint32_t inGLHandle);
	virtual void Bind(int32_t inTexNr) override;
	virtual void Unbind(int32_t inTexNr) override;

public:
	uint32_t GlHandle = 0;
};