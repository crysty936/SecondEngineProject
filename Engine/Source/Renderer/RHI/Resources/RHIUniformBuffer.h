#pragma once
#include "RHIBufferBase.h"

#include <stdint.h>
#include "Renderer/RHI/RHI.h"

class RHIUniformBuffer : public RHIBufferBase
{
public:
	RHIUniformBuffer(size_t inInitSize);

public:
	const size_t InitSize = 0;
	EShaderType BindingType = EShaderType::Sh_Vertex;
};