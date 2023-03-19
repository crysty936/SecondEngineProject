#include "RHIVertexBuffer.h"

RHIVertexBuffer::RHIVertexBuffer(eastl::shared_ptr<RHIIndexBuffer>& inIndexBuffer, const VertexInputLayout& inLayout)
	: RHIBufferBase(RHIBufferType::Vertex), IndexBuffer(inIndexBuffer), Layout(inLayout)
{

}
