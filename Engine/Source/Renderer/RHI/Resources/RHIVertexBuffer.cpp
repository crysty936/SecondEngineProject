#include "RHIVertexBuffer.h"

RHIVertexBuffer::RHIVertexBuffer(eastl::shared_ptr<RHIIndexBuffer>& inIndexBuffer, const VertexInputLayout& inLayout)
	: IndexBuffer(inIndexBuffer), Layout(inLayout)
{

}
