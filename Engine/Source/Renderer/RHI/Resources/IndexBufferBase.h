#pragma once
#include "RHIBufferBase.h"
#include <stdint.h>

class IndexBufferBase : public RHIBufferBase
{
public:
	int32_t IndexCount;
};