#include "RHIVertexBuffer.h"

RHIVertexBuffer::RHIVertexBuffer(eastl::shared_ptr<RHIIndexBuffer>& inIndexBuffer, const VertexBufferLayout& inLayout)
	: IndexBuffer(inIndexBuffer), Layout(inLayout)
{

}
