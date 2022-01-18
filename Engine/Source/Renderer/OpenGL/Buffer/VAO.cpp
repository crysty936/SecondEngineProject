#include "VAO.h"
#include "VertexBufferLayout.h"
#include "EASTL/vector.h"
#include "glad/glad.h"

VAO::VAO(VertexBuffer& inBuffer)
	: Handle{ 0 }, VBuffer{ inBuffer }
{
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

VAO::VAO()
{

}

VAO::~VAO() = default;

void VAO::Bind()
{
	glBindVertexArray(Handle);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::DeleteBuffers()
{
	glDeleteVertexArrays(1, &Handle);
	VBuffer.DeleteBuffer();
}
