#pragma once
#include "RHIBufferBase.h"
#include "VertexBufferLayout.h"
#include "EASTL/shared_ptr.h"
#include "IndexBufferBase.h"

class RHIVertexBuffer : public RHIBufferBase
{
public:
	RHIVertexBuffer(eastl::shared_ptr<class IndexBufferBase>& inIndexBuffer, const class VertexBufferLayout& inLayout);
	inline const VertexBufferLayout& GetLayout() const { return Layout; }
	inline int32_t GetIndicesCount() { return IndexBuffer->IndexCount; }
	virtual void ApplyLayout() = 0;

protected:
	eastl::shared_ptr<IndexBufferBase> IndexBuffer;
	VertexBufferLayout Layout;
};