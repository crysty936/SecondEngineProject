#pragma once
#include "Renderer/RHI/Resources/UniformBufferBase.h"
#include <stdint.h>

class GlUniformBuffer : public UniformBufferBase
{
public:
	GlUniformBuffer(const uint32_t inHandle);
	virtual void SetData(const char* inData, const size_t inSize) override;

private:
	uint32_t GLHandle = 0;
};