#pragma once
#include "Renderer/RHI/Resources/RHIUniformBuffer.h"

class D3D11UniformBuffer : public RHIUniformBuffer
{
public:
	D3D11UniformBuffer(struct ID3D11Buffer* inD3D11Handle, size_t inInitSize);

public:
	struct ID3D11Buffer* Handle;
};