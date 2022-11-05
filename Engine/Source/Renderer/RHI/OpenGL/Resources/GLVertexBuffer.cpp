#include "GLVertexBuffer.h"
#include "Renderer/RHI/Resources/RHIIndexBuffer.h"
#include "glad/glad.h"

GLVertexBuffer::GLVertexBuffer(uint32_t inGLHandle, eastl::shared_ptr<RHIIndexBuffer>& inIndexBuffer, const VertexInputLayout& inLayout)
	: RHIVertexBuffer(inIndexBuffer, inLayout), Handle(inGLHandle)
{}
