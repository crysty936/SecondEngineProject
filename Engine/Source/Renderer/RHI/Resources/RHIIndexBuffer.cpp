#include "RHIIndexBuffer.h"

RHIIndexBuffer::RHIIndexBuffer(int32_t inIndicesCount)
	: RHIBufferBase(RHIBufferType::Index), IndexCount(inIndicesCount)
{

}

RHIIndexBuffer::RHIIndexBuffer()
	: RHIBufferBase(RHIBufferType::Index)
{

}
