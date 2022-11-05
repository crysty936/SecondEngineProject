#include "RHIVertexBuffer.h"

RHIVertexBuffer::RHIVertexBuffer(eastl::shared_ptr<IndexBufferBase>& inIndexBuffer, const VertexBufferLayout& inLayout)
	: IndexBuffer(inIndexBuffer), Layout(inLayout)
{

}
