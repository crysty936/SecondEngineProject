#include "RHITextureBuffer.h"

RHITextureBuffer::RHITextureBuffer(size_t inInitSize)
	: RHIBufferBase(RHIBufferType::Uniform), InitSize(inInitSize)
{}

