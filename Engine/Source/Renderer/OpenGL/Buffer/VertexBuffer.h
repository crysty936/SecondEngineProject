#pragma once
#include "BufferBase.h"
#include <stdint.h>
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer/RenderingPrimitives.h"

class VertexBuffer : public BufferBase
{
public:
	VertexBuffer(const IndexBuffer& inIndices, const VertexBufferLayout& inLayout);
	VertexBuffer();
	virtual ~VertexBuffer();

	virtual void DeleteBuffer() override;

public:
	virtual void Bind() const override;
	virtual void Unbind() const override;

	void SetData(const float* inVertices, const int inCount, const uint32_t inBufferAccessType);
	void SetVertices(const eastl::vector<Vertex>& inVertices, const uint32_t inBufferAccessType);
	void SetVerticesRaw(const void* inData, const size_t inSize, const uint32_t inBufferAccessType);
	inline const VertexBufferLayout& GetLayout() const { return Layout; }
	inline uint32_t GetIndicesCount() const { return Indices.IndicesCount; }

private:
	IndexBuffer Indices;
	VertexBufferLayout Layout;
};