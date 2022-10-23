#include "OpenGLVertexBuffer.h"
#include "glad/glad.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(const OpenGLIndexBuffer& inIndices, const VertexBufferLayout& inLayout)
	: Indices{inIndices}, Layout{inLayout}
{
}

OpenGLVertexBuffer::OpenGLVertexBuffer() = default;
OpenGLVertexBuffer::~OpenGLVertexBuffer() = default;

void OpenGLVertexBuffer::DeleteBuffer()
{
	BufferBase::DeleteBuffer();
	Indices.DeleteBuffer();
}

void OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
	Indices.Bind();
}

void OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Indices.Unbind();
}

void OpenGLVertexBuffer::SetData(const float* inVertices, const int inCount)
{
	Bind();
	glNamedBufferData(Handle, sizeof(float) * inCount, inVertices, GL_STATIC_DRAW);
	Unbind();
}

void OpenGLVertexBuffer::SetVertices(const eastl::vector<Vertex>& inVertices)
{
	Bind();
	const int32_t verticesCount = static_cast<int32_t>(inVertices.size());
	glNamedBufferData(Handle, sizeof(Vertex) * verticesCount, inVertices.data(), GL_STATIC_DRAW);
	Unbind();
}

void OpenGLVertexBuffer::SetVerticesRaw(const void* inData, const size_t inSize)
{
	glBufferData(Handle, inSize, inData, GL_STATIC_DRAW);
}

