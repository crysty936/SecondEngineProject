#include "RHIUniformBuffer.h"

RHIUniformBuffer::RHIUniformBuffer(size_t inInitSize)
	: RHIBufferBase(RHIBufferType::Uniform), InitSize(inInitSize)
{}

