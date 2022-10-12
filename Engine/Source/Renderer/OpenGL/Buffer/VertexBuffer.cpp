#include "VertexBuffer.h"
#include "glad/glad.h"

VertexBuffer::VertexBuffer(const IndexBuffer& inIndices, const VertexBufferLayout& inLayout)
	: Indices{inIndices}, Layout{inLayout}
{
}

VertexBuffer::VertexBuffer() = default;
VertexBuffer::~VertexBuffer() = default;

void VertexBuffer::DeleteBuffer()
{
	BufferBase::DeleteBuffer();
	Indices.DeleteBuffer();
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
	Indices.Bind();
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Indices.Unbind();
}

void VertexBuffer::SetData(const float* inVertices, const int inCount, const uint32_t inBufferAccessType)
{
	Bind();
	glNamedBufferData(Handle, sizeof(float) * inCount, inVertices, inBufferAccessType);
	Unbind();
}

void VertexBuffer::SetVertices(const eastl::vector<Vertex>& inVertices, const uint32_t inBufferAccessType)
{
	Bind();
	const int32_t verticesCount = static_cast<int32_t>(inVertices.size());
	glNamedBufferData(Handle, sizeof(Vertex) * verticesCount, inVertices.data(), inBufferAccessType);
	Unbind();
}

void VertexBuffer::SetVerticesRaw(const void* inData, const size_t inSize, const uint32_t inBufferAccessType)
{
	glBufferData(Handle, inSize, inData, inBufferAccessType);
}

