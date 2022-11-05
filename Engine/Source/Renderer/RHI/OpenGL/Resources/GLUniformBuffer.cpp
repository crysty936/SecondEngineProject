#include "GLUniformBuffer.h"
#include "glad/glad.h"

GlUniformBuffer::GlUniformBuffer(const uint32_t inHandle, size_t inInitSize)
	: RHIUniformBuffer(inInitSize), GLHandle(inHandle)
{}
