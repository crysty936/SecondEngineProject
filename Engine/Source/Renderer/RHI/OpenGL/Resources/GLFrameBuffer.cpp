#include "GLFrameBuffer.h"
#include "glad/glad.h"

GLFrameBuffer::GLFrameBuffer(const uint32_t inHandle)
	: RHIFrameBuffer(), GLHandle(inHandle)
{}

bool GLFrameBuffer::HasDepthAttachment() const
{
	return DepthAttachment != nullptr;
}

bool GLFrameBuffer::HasColorAttachment() const
{
	return ColorAttachments.size() != 0;
}
