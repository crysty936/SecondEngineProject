#pragma once
#include "Renderer/RHI/Resources/RHIFrameBuffer.h"
#include <stdint.h>

class GLFrameBuffer : public RHIFrameBuffer
{
public:
	GLFrameBuffer(const uint32_t inHandle);


	virtual bool HasDepthStencilAttachment() const override;
	virtual bool HasColorAttachment() const override;

public:
	uint32_t GLHandle = 0;
	uint32_t DepthStencingAttachment = 0;
	uint32_t ColorAttachment = 0;
};