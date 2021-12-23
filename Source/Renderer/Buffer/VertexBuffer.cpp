#include "VertexBuffer.h"
#include "glad/glad.h"

VertexBuffer::VertexBuffer() = default;
VertexBuffer::~VertexBuffer() = default;

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(const float* inVertices, const int inCount, const uint32_t inDrawType)
{
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * inCount, inVertices, inDrawType);
}

void VertexBuffer::SetData(const void* inData, const size_t inSize, const uint32_t inDrawType)
{
	glBufferData(GL_ARRAY_BUFFER, inSize, inData, inDrawType);
}

