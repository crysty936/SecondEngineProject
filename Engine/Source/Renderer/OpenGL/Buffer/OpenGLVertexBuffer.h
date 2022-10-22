#pragma once
#include "BufferBase.h"
#include <stdint.h>
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexBufferLayout.h"
#include "Renderer/RenderingPrimitives.h"

class OpenGLVertexBuffer : public BufferBase
{
public:
	OpenGLVertexBuffer(const OpenGLIndexBuffer& inIndices, const OpenGLVertexBufferLayout& inLayout);
	OpenGLVertexBuffer();
	virtual ~OpenGLVertexBuffer();

	virtual void DeleteBuffer() override;

public:
	virtual void Bind() const override;
	virtual void Unbind() const override;

	void SetData(const float* inVertices, const int inCount, const uint32_t inBufferAccessType);
	void SetVertices(const eastl::vector<Vertex>& inVertices, const uint32_t inBufferAccessType);
	void SetVerticesRaw(const void* inData, const size_t inSize, const uint32_t inBufferAccessType);
	inline const OpenGLVertexBufferLayout& GetLayout() const { return Layout; }
	inline uint32_t GetIndicesCount() const { return Indices.IndicesCount; }

private:
	OpenGLIndexBuffer Indices;
	OpenGLVertexBufferLayout Layout;
};