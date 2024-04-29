#pragma once
#include "Renderer/RHI/Resources/RHIBuffers.h"

class D3D11IndexBuffer : public RHIIndexBuffer
{
public:
	D3D11IndexBuffer(struct ID3D11Buffer* inD3DHandle, const int32_t inIndexCount);

public:
	struct ID3D11Buffer* Handle = 0;
};