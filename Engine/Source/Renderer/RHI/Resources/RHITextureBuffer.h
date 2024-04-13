#pragma once
#include "RHIBufferBase.h"

#include <stdint.h>
#include "Renderer/RHI/RHI.h"

// One dimensional Array
class RHITextureBuffer : public RHIBufferBase
{
public:
	RHITextureBuffer(size_t inInitSize);

public:
	const size_t InitSize = 0;
	EShaderType BindingType = EShaderType::Sh_Vertex;
};