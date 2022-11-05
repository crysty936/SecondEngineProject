#pragma once
#include "Renderer/RHI/Resources/RHIUniformBuffer.h"
#include <stdint.h>

class GlUniformBuffer : public RHIUniformBuffer
{
public:
	GlUniformBuffer(const uint32_t inHandle, size_t inInitSize);

public:
	uint32_t GLHandle = 0;
};