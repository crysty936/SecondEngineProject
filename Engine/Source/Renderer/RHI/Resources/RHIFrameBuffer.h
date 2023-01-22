#pragma once
#include <stdint.h>
#include "EASTL/string.h"

class RHIFrameBuffer
{
public:
	virtual bool HasDepthStencilAttachment() const = 0;
	virtual bool HasColorAttachment() const = 0;
};