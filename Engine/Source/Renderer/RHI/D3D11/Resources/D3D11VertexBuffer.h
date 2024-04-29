#pragma once
#include "Renderer/RHI/Resources/RHIBuffers.h"

class D3D11VertexBuffer : public RHIVertexBuffer
{
public:
	D3D11VertexBuffer(struct ID3D11Buffer* inD3D11Handle, eastl::shared_ptr<RHIIndexBuffer>& inIndexBuffer, const VertexInputLayout& inLayout);

public:
	struct ID3D11Buffer* Handle = 0;
};