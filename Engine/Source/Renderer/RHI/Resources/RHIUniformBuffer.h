#pragma once
#include "RHIBufferBase.h"

#include <stdint.h>

enum class ConstantBufferType
{
	Vertex,
	Geometry,
	Pixel
};

class RHIUniformBuffer : public RHIBufferBase
{
public:
	RHIUniformBuffer(size_t inInitSize);

public:
	const size_t InitSize = 0;
	ConstantBufferType BType = ConstantBufferType::Vertex;
};