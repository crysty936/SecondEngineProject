#pragma once
#include "BufferBase.h"
#include <stdint.h>
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexBuffer : public BufferBase
{
public:
	VertexBuffer(const IndexBuffer& inIndices, const VertexBufferLayout& inLayout);
	virtual ~VertexBuffer();

	virtual void DeleteBuffer() override;

public:
	virtual void Bind() override;
	virtual void Unbind() override;

	void SetVertices(const float* inVertices, const int inCount, const uint32_t inBufferAccessType);
	void SetVerticesRaw(const void* inData, const size_t inSize, const uint32_t inBufferAccessType);
	inline VertexBufferLayout& GetLayout() { return Layout; }
	inline uint32_t GetIndicesCount() const { return Indices.IndicesCount; }

private:
	IndexBuffer Indices;
	VertexBufferLayout Layout;
};