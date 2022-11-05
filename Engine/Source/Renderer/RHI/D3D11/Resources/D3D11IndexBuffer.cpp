#include "D3D11IndexBuffer.h"
#include <d3d11.h>

D3D11IndexBuffer::D3D11IndexBuffer(ID3D11Buffer* inD3DHandle, const int32_t inIndexCount)
	: RHIIndexBuffer(inIndexCount), Handle(inD3DHandle)
{}

