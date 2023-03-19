#include "RHIIndexBuffer.h"

RHIIndexBuffer::RHIIndexBuffer(int32_t inIndicesCount)
	: RHIBufferBase(RHIBufferType::Index), IndexCount(inIndicesCount)
{

}
