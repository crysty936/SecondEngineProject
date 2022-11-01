#pragma once
#include "Renderer/RHI/Resources/VertexBufferBase.h"

class GLVertexBuffer : public VertexBufferBase
{
public:
	GLVertexBuffer(uint32_t inGLHandle, eastl::shared_ptr<IndexBufferBase>& inIndexBuffer, const VertexBufferLayout& inLayout);
	virtual void Bind() override;
	virtual void Unbind() override;

public:
	uint32_t Handle = 0;
};