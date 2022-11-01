#pragma once
#include "RHIBufferBase.h"
#include "VertexBufferLayout.h"
#include "EASTL/shared_ptr.h"
#include "IndexBufferBase.h"

class VertexBufferBase : public RHIBufferBase
{
public:
	VertexBufferBase(eastl::shared_ptr<class IndexBufferBase>& inIndexBuffer, const class VertexBufferLayout& inLayout);
	inline const VertexBufferLayout& GetLayout() const { return Layout; }
	inline int32_t GetIndicesCount() { return IndexBuffer->IndexCount; }

protected:
	eastl::shared_ptr<IndexBufferBase> IndexBuffer;
	VertexBufferLayout Layout;
};