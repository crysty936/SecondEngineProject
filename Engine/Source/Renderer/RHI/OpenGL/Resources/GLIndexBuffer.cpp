#include "GLIndexBuffer.h"
#include "glad/glad.h"

GLIndexBuffer::GLIndexBuffer(const uint32_t inGLHandle, const int32_t inIndexCount)
	: RHIIndexBuffer(inIndexCount), Handle(inGLHandle)
{}

