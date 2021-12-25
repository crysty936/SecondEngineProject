#include "BufferBase.h"
#include "glad/glad.h"

BufferBase::BufferBase()
{
	glGenBuffers(1, &Handle);
}

BufferBase::~BufferBase() = default;

void BufferBase::DeleteBuffer()
{
	if (Handle != 0)
	{
		glDeleteBuffers(1, &Handle);
	}
}
