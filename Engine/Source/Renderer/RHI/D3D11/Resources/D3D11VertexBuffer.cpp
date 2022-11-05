#include "D3D11VertexBuffer.h"
#include "Renderer/RHI/Resources/RHIIndexBuffer.h"

D3D11VertexBuffer::D3D11VertexBuffer(ID3D11Buffer* inD3D11Handle, eastl::shared_ptr<RHIIndexBuffer>& inIndexBuffer, const VertexInputLayout& inLayout)
	: RHIVertexBuffer(inIndexBuffer, inLayout), Handle(inD3D11Handle)
{}
