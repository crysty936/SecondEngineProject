#pragma once
#include "RHIBufferBase.h"
#include <stdint.h>

class RHIIndexBuffer : public RHIBufferBase
{
public:
	RHIIndexBuffer(int32_t inIndicesCount);

public:
	int32_t IndexCount;
};