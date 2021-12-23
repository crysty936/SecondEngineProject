#pragma once
#include "BufferBase.h"
#include <stdint.h>

class VertexBuffer : public BufferBase
{
public:
	VertexBuffer();
	virtual ~VertexBuffer();

public:
	virtual void Bind() override;
	virtual void Unbind() override;

	void SetData(const float* inVertices, const int inCount, const uint32_t inDrawType);
	void SetData(const void* inData, const size_t inSize, const uint32_t inDrawType);
};