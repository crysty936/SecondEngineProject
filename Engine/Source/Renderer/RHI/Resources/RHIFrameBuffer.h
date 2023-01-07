#pragma once
#include <stdint.h>
#include "EASTL/string.h"

class RHIFrameBuffer
{
public:
	bool HasDepthStencilAttachment = false;
	bool HasColorAttachment = false;
};