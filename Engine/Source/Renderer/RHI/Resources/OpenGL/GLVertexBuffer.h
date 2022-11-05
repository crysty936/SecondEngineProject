#pragma once
#include "Renderer/RHI/Resources/RHIVertexBuffer.h"

class GLVertexBuffer : public RHIVertexBuffer
{
public:
	GLVertexBuffer(uint32_t inGLHandle, eastl::shared_ptr<IndexBufferBase>& inIndexBuffer, const VertexBufferLayout& inLayout);
	virtual void Bind() override;
	virtual void Unbind() override;

	virtual void ApplyLayout() override;

public:
	uint32_t Handle = 0;
};