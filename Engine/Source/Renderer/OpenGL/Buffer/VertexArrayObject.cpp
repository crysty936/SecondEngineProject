#include "VertexArrayObject.h"
#include "VertexBufferLayout.h"
#include "EASTL/vector.h"
#include "glad/glad.h"

VertexArrayObject::VertexArrayObject()
	: Handle{ 0 }
{

}


VertexArrayObject::~VertexArrayObject() = default;

void VertexArrayObject::SetupState()
{
	// Set up the VAO state
	glGenVertexArrays(1, &Handle);

	Bind();
	VBuffer.Bind();

	const VertexBufferLayout& layout = VBuffer.GetLayout();
	const eastl::vector<LayoutProperties>& props = layout.GetProperties();

	size_t offset = 0;
	for (int32_t i = 0; i < props.size(); i++)
	{
		const LayoutProperties& prop = props[i];

		void* offsetPtr = reinterpret_cast<void*>(offset);
		glVertexAttribPointer(i, prop.Count, prop.Type, prop.bNormalized, layout.GetStride(), offsetPtr);
		glEnableVertexAttribArray(i);

		offset += prop.Count * prop.GetSizeOfType();
	}

	Unbind();
	VBuffer.Unbind();
}

void VertexArrayObject::Bind() const
{
	ASSERT(Handle != 0);

	glBindVertexArray(Handle);
}

void VertexArrayObject::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArrayObject::DeleteBuffers()
{
	glDeleteVertexArrays(1, &Handle);
	VBuffer.DeleteBuffer();
}
