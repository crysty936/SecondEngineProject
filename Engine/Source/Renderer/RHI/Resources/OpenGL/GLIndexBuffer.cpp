#include "GLIndexBuffer.h"
#include "glad/glad.h"

GLIndexBuffer::GLIndexBuffer(const uint32_t inGLHandle, const int32_t inIndexCount)
	: IndexBufferBase(inIndexCount), Handle(inGLHandle)
{}

void GLIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle);
}

void GLIndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
