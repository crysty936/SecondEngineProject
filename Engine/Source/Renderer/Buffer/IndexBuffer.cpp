#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
	: IndicesCount{ 0 }
{
}

IndexBuffer::~IndexBuffer() = default;

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle);
	IsBound = true;
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	IsBound = false;
}

void IndexBuffer::SetIndices(const uint32_t* inIndices, const int32_t inCount, const GLenum inBufferAccessType)
{
	glNamedBufferData(Handle, sizeof(uint32_t) * inCount, inIndices, inBufferAccessType);
	IndicesCount = inCount;
}
