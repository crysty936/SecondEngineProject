#include "VertexArrayObject.h"
#include "EASTL/vector.h"
#include "glad/glad.h"
#include "Renderer/RHI/Resources/VertexBufferLayout.h"

VertexArrayObject::VertexArrayObject()
	: Handle{ 0 }
{

}

VertexArrayObject::~VertexArrayObject() = default;

void VertexArrayObject::SetupState() const
{
	// Set up the VAO state
	glGenVertexArrays(1, &Handle);

	Bind();
	VBuffer.Bind();

	const VertexBufferLayout& layout = VBuffer.GetLayout();
	const eastl::vector<VertexLayoutProperties>& props = layout.GetProperties();

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

		glVertexAttribPointer(i, prop.Count, glType, prop.bNormalized, layout.GetStride(), offsetPtr);
		glEnableVertexAttribArray(i);

		offset += prop.Count * prop.GetSizeOfType();
	}

	Unbind();
	VBuffer.Unbind();

	bReadyForDraw = true;
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
