#include "OpenGLIndexBuffer.h"
#include "glad/glad.h"

OpenGLIndexBuffer::OpenGLIndexBuffer()
	: IndicesCount{ 0 }
{
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() = default;

void OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle);
	IsBound = true;
}

void OpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	IsBound = false;
}

void OpenGLIndexBuffer::SetIndices(const uint32_t* inIndices, const int32_t inCount, const uint32_t inBufferAccessType)
{
	Bind();
	glNamedBufferData(Handle, sizeof(uint32_t) * inCount, inIndices, inBufferAccessType);
	Unbind();
	IndicesCount = inCount;
}
