#pragma once

#include "BufferBase.h"
#include "glad/glad.h"

class IndexBuffer : public BufferBase
{
public:
	IndexBuffer();
	virtual ~IndexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;
	void SetIndices(const uint32_t* inIndices, const int32_t inCount, const GLenum inBufferAccessType);

public:
	int32_t IndicesCount;
};