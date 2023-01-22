#include "GLFrameBuffer.h"
#include "glad/glad.h"

GLFrameBuffer::GLFrameBuffer(const uint32_t inHandle)
	: RHIFrameBuffer(), GLHandle(inHandle)
{}

bool GLFrameBuffer::HasDepthStencilAttachment() const
{
	return DepthStencingAttachment != 0;
}

bool GLFrameBuffer::HasColorAttachment() const
{
	return ColorAttachment != 0;
}
