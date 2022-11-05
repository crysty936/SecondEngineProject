#include "GLVertexBuffer.h"
#include "Renderer/RHI/Resources/IndexBufferBase.h"
#include "glad/glad.h"

GLVertexBuffer::GLVertexBuffer(uint32_t inGLHandle, eastl::shared_ptr<IndexBufferBase>& inIndexBuffer, const VertexBufferLayout& inLayout)
	: RHIVertexBuffer(inIndexBuffer, inLayout), Handle(inGLHandle)
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

void GLVertexBuffer::ApplyLayout()
{
	const eastl::vector<VertexLayoutProperties>& props = Layout.GetProperties();

	size_t offset = 0;
	for (int32_t i = 0; i < props.size(); i++)
	{
		const VertexLayoutProperties& prop = props[i];

		void* offsetPtr = reinterpret_cast<void*>(offset);
		uint32_t glType = 0;

		switch (prop.Type)
		{
		case VertexPropertyType::UInt:
			glType = GL_UNSIGNED_INT;
			break;
		case VertexPropertyType::Float:
			glType = GL_FLOAT;
			break;
		}

		glVertexAttribPointer(i, prop.Count, glType, prop.bNormalized, Layout.GetStride(), offsetPtr);
		glEnableVertexAttribArray(i);

		offset += prop.Count * prop.GetSizeOfType();
	}

}
