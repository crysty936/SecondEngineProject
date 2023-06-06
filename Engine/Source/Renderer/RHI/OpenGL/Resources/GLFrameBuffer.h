#pragma once
#include "Renderer/RHI/Resources/RHIFrameBuffer.h"
#include <stdint.h>

class GLFrameBuffer : public RHIFrameBuffer
{
public:
	GLFrameBuffer(const uint32_t inHandle);

	virtual bool HasDepthAttachment() const override;
	virtual bool HasColorAttachment() const override;

public:
	uint32_t GLHandle = 0;
};