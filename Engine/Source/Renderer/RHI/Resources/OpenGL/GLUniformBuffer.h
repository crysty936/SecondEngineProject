#pragma once
#include "Renderer/RHI/Resources/RHIUniformBuffer.h"
#include <stdint.h>

class GlUniformBuffer : public RHIUniformBuffer
{
public:
	GlUniformBuffer(const uint32_t inHandle);
	virtual void SetData(const char* inData, const size_t inSize) override;

private:
	uint32_t GLHandle = 0;
};