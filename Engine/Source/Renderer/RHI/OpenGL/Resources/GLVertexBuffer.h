#pragma once
#include "Renderer/RHI/Resources/RHIVertexBuffer.h"

class GLVertexBuffer : public RHIVertexBuffer
{
public:
	GLVertexBuffer(uint32_t inGLHandle, eastl::shared_ptr<RHIIndexBuffer>& inIndexBuffer, const VertexInputLayout& inLayout);

public:
	uint32_t Handle = 0;
};