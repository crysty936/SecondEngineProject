#pragma once
#include "Renderer/RHI/Resources/RHIBuffers.h"

class GLIndexBuffer : public RHIIndexBuffer
{
public:
	GLIndexBuffer(const uint32_t inGLHandle, const int32_t inIndexCount);

public:
	uint32_t Handle = 0;
};