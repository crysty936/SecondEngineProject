#pragma once
#include "RHIBufferBase.h"

#include <stdint.h>

class UniformBufferBase : public RHIBufferBase
{
public:
	virtual void SetData(const char* inData, const size_t inSize) = 0;
};