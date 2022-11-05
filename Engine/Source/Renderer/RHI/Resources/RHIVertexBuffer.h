#pragma once
#include "RHIBufferBase.h"
#include "VertexBufferLayout.h"
#include "EASTL/shared_ptr.h"
#include "RHIIndexBuffer.h"

class RHIVertexBuffer : public RHIBufferBase
{
public:
	RHIVertexBuffer(eastl::shared_ptr<class RHIIndexBuffer>& inIndexBuffer, const class VertexBufferLayout& inLayout);
	inline const VertexBufferLayout& GetLayout() const { return Layout; }
	inline int32_t GetIndicesCount() { return IndexBuffer->IndexCount; }

public:
	eastl::shared_ptr<RHIIndexBuffer> IndexBuffer;
	VertexBufferLayout Layout;
};