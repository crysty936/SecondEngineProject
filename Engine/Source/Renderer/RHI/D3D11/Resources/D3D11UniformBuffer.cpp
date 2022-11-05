#include "D3D11UniformBuffer.h"

D3D11UniformBuffer::D3D11UniformBuffer(ID3D11Buffer* inD3D11Handle, size_t inInitSize)
	: RHIUniformBuffer(inInitSize),  Handle(inD3D11Handle)
{}

