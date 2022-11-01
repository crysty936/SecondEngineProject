#pragma once
#include "Renderer/RHI/Resources/IndexBufferBase.h"

class GLIndexBuffer : public IndexBufferBase
{
public:
	GLIndexBuffer(const uint32_t inGLHandle, const int32_t inIndexCount);
	virtual void Bind() override;
	virtual void Unbind() override;

public:
	uint32_t Handle = 0;
};