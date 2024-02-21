#include "D3D12Resources.h"
#include "D3D12Utility.h"

D3D12IndexBuffer::D3D12IndexBuffer()
	: RHIIndexBuffer()
{}

D3D12_INDEX_BUFFER_VIEW D3D12IndexBuffer::IBView() const
{
	D3D12_INDEX_BUFFER_VIEW ibView;
	ibView.BufferLocation = GPUAddress;
	ibView.Format = IBFormat;
	ibView.SizeInBytes = Size;

	return ibView;
}

D3D12VertexBuffer::D3D12VertexBuffer()
	: RHIVertexBuffer()
{}

D3D12_VERTEX_BUFFER_VIEW D3D12VertexBuffer::VBView() const
{
	D3D12_VERTEX_BUFFER_VIEW outView = {};

	outView.BufferLocation = GPUAddress;
	outView.StrideInBytes = Layout.GetStride();
	//outView.StrideInBytes = 3;// 8 floats
	outView.SizeInBytes = AllocatedSize;

	return outView;
}
