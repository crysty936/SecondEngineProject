#pragma once
#include "BufferBase.h"
#include <stdint.h>
#include "OpenGLIndexBuffer.h"
#include "Renderer/RenderingPrimitives.h"
#include "Renderer/RHI/Resources/VertexBufferLayout.h"

class OpenGLVertexBuffer : public BufferBase
{
public:
	OpenGLVertexBuffer(const OpenGLIndexBuffer& inIndices, const VertexBufferLayout& inLayout);
	OpenGLVertexBuffer();
	virtual ~OpenGLVertexBuffer();

	virtual void DeleteBuffer() override;

public:
	virtual void Bind() const override;
	virtual void Unbind() const override;

	void SetData(const float* inVertices, const int inCount);
	void SetVertices(const eastl::vector<Vertex>& inVertices);
	void SetVerticesRaw(const void* inData, const size_t inSize);
	inline const VertexBufferLayout& GetLayout() const { return Layout; }
	inline uint32_t GetIndicesCount() const { return Indices.IndicesCount; }

private:
	OpenGLIndexBuffer Indices;
	VertexBufferLayout Layout;
};