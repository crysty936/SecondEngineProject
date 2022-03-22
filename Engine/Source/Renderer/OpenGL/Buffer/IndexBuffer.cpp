#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
	: IndicesCount{ 0 }
{
}

IndexBuffer::~IndexBuffer() = default;

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle);
	IsBound = true;
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	IsBound = false;
}

void IndexBuffer::SetIndices(const uint32_t* inIndices, const int32_t inCount, const GLenum inBufferAccessType)
{
	Bind();
	glNamedBufferData(Handle, sizeof(uint32_t) * inCount, inIndices, inBufferAccessType);
	Unbind();
	IndicesCount = inCount;
}
