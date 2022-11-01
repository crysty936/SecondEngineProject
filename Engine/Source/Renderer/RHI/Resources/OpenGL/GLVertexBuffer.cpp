#include "GLVertexBuffer.h"
#include "Renderer/RHI/Resources/IndexBufferBase.h"
#include "glad/glad.h"

GLVertexBuffer::GLVertexBuffer(uint32_t inGLHandle, eastl::shared_ptr<IndexBufferBase>& inIndexBuffer, const VertexBufferLayout& inLayout)
	: VertexBufferBase(inIndexBuffer, inLayout), Handle(inGLHandle)
{}

void GLVertexBuffer::Bind()
{
	IndexBuffer->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, Handle);
}

void GLVertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	IndexBuffer->Unbind();
}
