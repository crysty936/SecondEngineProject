#include "GLFrameBuffer.h"
#include "glad/glad.h"

GLFrameBuffer::GLFrameBuffer(const uint32_t inHandle)
	: RHIFrameBuffer(), GLHandle(inHandle)
{}
