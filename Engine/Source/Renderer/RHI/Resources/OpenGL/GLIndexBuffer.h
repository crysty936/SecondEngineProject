#pragma once
#include "Renderer/RHI/Resources/IndexBufferBase.h"

class GLIndexBuffer : public IndexBufferBase
{
public:
	uint32_t Handle = 0;
};