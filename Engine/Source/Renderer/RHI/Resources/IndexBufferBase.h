#pragma once
#include "RHIBufferBase.h"
#include <stdint.h>

class IndexBufferBase : public RHIBufferBase
{
public:
	IndexBufferBase(int32_t inIndicesCount);

public:
	int32_t IndexCount;
};