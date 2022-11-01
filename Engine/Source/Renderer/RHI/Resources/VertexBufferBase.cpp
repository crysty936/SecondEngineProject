#include "VertexBufferBase.h"

VertexBufferBase::VertexBufferBase(eastl::shared_ptr<IndexBufferBase>& inIndexBuffer, const VertexBufferLayout& inLayout)
	: IndexBuffer(inIndexBuffer), Layout(inLayout)
{

}
