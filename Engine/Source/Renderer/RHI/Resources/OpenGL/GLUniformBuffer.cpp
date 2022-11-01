#include "GLUniformBuffer.h"
#include "glad/glad.h"

GlUniformBuffer::GlUniformBuffer(const uint32_t inHandle)
	: GLHandle(inHandle)
{}

void GlUniformBuffer::SetData(const char* inData, const size_t inSize)
{
	glBindBuffer(GL_UNIFORM_BUFFER, GLHandle);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, GLHandle);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, inSize, inData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

