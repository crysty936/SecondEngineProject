#include "GLTextureBuffer.h"
#include "glad/glad.h"

GlTextureBuffer::GlTextureBuffer(const uint32_t inHandle, const uint32_t inBufferHandle, size_t inInitSize)
	: RHITextureBuffer(inInitSize), GLTextureHandle(inHandle), GLBufferHandle(inBufferHandle)
{}
