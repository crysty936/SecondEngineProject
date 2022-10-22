#pragma once

#include "BufferBase.h"

class OpenGLIndexBuffer : public BufferBase
{
public:
	OpenGLIndexBuffer();
	virtual ~OpenGLIndexBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;
	void SetIndices(const uint32_t* inIndices, const int32_t inCount, const uint32_t inBufferAccessType);

public:
	int32_t IndicesCount;
};