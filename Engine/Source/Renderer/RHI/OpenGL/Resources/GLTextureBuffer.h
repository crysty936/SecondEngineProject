#pragma once
#include "Renderer/RHI/Resources/RHIBuffers.h"
#include <stdint.h>

class GlTextureBuffer : public RHITextureBuffer
{
public:
	GlTextureBuffer(const uint32_t inHandle, const uint32_t inBufferHandle, size_t inInitSize);

public:
	uint32_t GLTextureHandle = 0;
	uint32_t GLBufferHandle = 0;
};