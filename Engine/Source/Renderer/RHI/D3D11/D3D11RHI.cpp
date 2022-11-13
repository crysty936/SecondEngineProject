#include "D3D11RHI.h"
#include <windows.h>

#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"
#include "Window/WindowsWindow.h"

#include <windows.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include "Renderer/RHI/D3D11/Resources/D3D11IndexBuffer.h"
#include "Renderer/RHI/D3D11/Resources/D3D11VertexBuffer.h"
#include "Renderer/RHI/D3D11/Resources/D3D11UniformBuffer.h"
#include "Renderer/RHI/D3D11/Resources/D3D11Shader.h"
#include "Renderer/RHI/D3D11/Resources/D3D11UniformBuffer.h"
#include "Renderer/RHI/D3D11/Resources/D3D11Texture2D.h"
#include "Utils/IOUtils.h"
#include "Utils/ImageLoading.h"

static D3D_DRIVER_TYPE	PresentDriverType = D3D_DRIVER_TYPE_NULL;
static D3D_FEATURE_LEVEL PresentFeatureLevel = D3D_FEATURE_LEVEL_11_0;
static ID3D11Device* D3DDevice = NULL;
static ID3D11DeviceContext* ImmediateContext = NULL;
static IDXGISwapChain* SwapChain = NULL;
static ID3D11RenderTargetView* RenderTarget = NULL;
static ID3D11SamplerState* GlobalSamplerLinear = NULL;

D3D11RHI::D3D11RHI()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(static_cast<HWND>(Engine->GetMainWindow().GetHandle()), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = static_cast<HWND>(Engine->GetMainWindow().GetHandle());
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		PresentDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, PresentDriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &SwapChain, &D3DDevice, &PresentFeatureLevel, &ImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	ASSERT(!FAILED(hr));

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	ASSERT(!FAILED(hr));

	hr = D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &RenderTarget);
	pBackBuffer->Release();
	ASSERT(!FAILED(hr));

	ImmediateContext->OMSetRenderTargets(1, &RenderTarget, NULL);

	// Setup the viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	ImmediateContext->RSSetViewports(1, &vp);

	// Set primitive topology
	ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Change Winding Order
	{
		D3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.FrontCounterClockwise = true; // Change winding order to keep similar vertices between API's
		rastDesc.DepthBias = 0;
		rastDesc.SlopeScaledDepthBias = 0.0f;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.DepthClipEnable = true;
		rastDesc.ScissorEnable = false;
		rastDesc.MultisampleEnable = false;
		rastDesc.AntialiasedLineEnable = false;

		ID3D11RasterizerState* newState = nullptr;

		D3DDevice->CreateRasterizerState(&rastDesc, &newState);
		ImmediateContext->RSSetState(newState);
	}

	// Create the global linear sample state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = D3DDevice->CreateSamplerState(&samplerDesc, &GlobalSamplerLinear);
	ASSERT(!FAILED(hr));
}

D3D11RHI::~D3D11RHI() = default;

eastl::shared_ptr<RHIVertexBuffer> D3D11RHI::CreateVertexBuffer(const VertexInputLayout& inLayout, const float* inVertices, const int32_t inCount, eastl::shared_ptr<RHIIndexBuffer> inIndexBuffer)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* bufferHandle = nullptr;
	D3D11_BUFFER_DESC bd = {};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(float) * inCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = inVertices;

	hr = D3DDevice->CreateBuffer(&bd, &initData, &bufferHandle);
	ASSERT(!FAILED(hr));

	eastl::shared_ptr<D3D11VertexBuffer> newBuffer = eastl::make_shared<D3D11VertexBuffer>(bufferHandle, inIndexBuffer, inLayout);

	return newBuffer;
}

eastl::shared_ptr<RHIVertexBuffer> D3D11RHI::CreateVertexBuffer(const VertexInputLayout& inLayout, const eastl::vector<Vertex>& inVertices, eastl::shared_ptr<RHIIndexBuffer> inIndexBuffer)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* bufferHandle = nullptr;
	D3D11_BUFFER_DESC bd = {};

	const int32_t verticesCount = static_cast<int32_t>(inVertices.size());
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(float) * verticesCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = inVertices.data();

	hr = D3DDevice->CreateBuffer(&bd, &initData, &bufferHandle);
	ASSERT(!FAILED(hr));

	eastl::shared_ptr<D3D11VertexBuffer> newBuffer = eastl::make_shared<D3D11VertexBuffer>(bufferHandle, inIndexBuffer, inLayout);

	return newBuffer;
}

eastl::shared_ptr<RHIIndexBuffer> D3D11RHI::CreateIndexBuffer(const uint32_t* inData, uint32_t inCount)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* bufferHandle = nullptr;
	D3D11_BUFFER_DESC ibd = {};

	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(int32_t) * inCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = inData;
	hr = D3DDevice->CreateBuffer(&ibd, &initData, &bufferHandle);
	ASSERT(!FAILED(hr));

	eastl::shared_ptr<D3D11IndexBuffer> newBuffer = eastl::make_shared<D3D11IndexBuffer>(bufferHandle, inCount);

	return newBuffer;
}

eastl::shared_ptr<class RHIUniformBuffer> D3D11RHI::CreateUniformBuffer(size_t inSize)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* bufferHandle = nullptr;
	D3D11_BUFFER_DESC bd = {};

	const uint32_t usize = static_cast<uint32_t>(inSize);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = usize;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	hr = D3DDevice->CreateBuffer(&bd, NULL, &bufferHandle);
	ASSERT(!FAILED(hr));

	eastl::shared_ptr<D3D11UniformBuffer> newBuffer = eastl::make_shared<D3D11UniformBuffer>(bufferHandle, inSize);

	return newBuffer;
}

eastl::shared_ptr<class RHITexture2D> D3D11RHI::CreateTexture2D(const eastl::string& inDataPath)
{
	// TODO: Create empty texture and they update it using UpdateSubresource

	ImageData data = ImageLoading::LoadImageData(inDataPath.data());
	if (!data.RawData)
 	{
 		return nullptr;
 	}

  	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = data.Width;
	desc.Height = data.Height;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	HRESULT hr = S_OK;
 	ID3D11Texture2D* texHandle = nullptr;
	//hr = D3DDevice->CreateTexture2D(&desc, &srd, &texHandle);
	hr = D3DDevice->CreateTexture2D(&desc, nullptr, &texHandle);
	ASSERT(!FAILED(hr));

	uint32_t rowPitch = (data.Width * 4) * sizeof(unsigned char);

	ImmediateContext->UpdateSubresource(texHandle, 0, nullptr, data.RawData, rowPitch, 0);

 	eastl::shared_ptr<D3D11Texture2D> newTex = eastl::make_shared<D3D11Texture2D>(texHandle);

	ImageLoading::FreeImageData(data);

 	return newTex;
}

void D3D11RHI::BindTexture2D(const RHITexture2D& inTex, const int32_t inTexId)
{
	const D3D11Texture2D& d3d11Tex = static_cast<const D3D11Texture2D&>(inTex);
	D3D11Texture2D& nonconst_d3d11Tex = const_cast<D3D11Texture2D&>(d3d11Tex);

	if (!d3d11Tex.ResourceViewHandle)
	{
		HRESULT hr = S_OK;
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = -1;
		hr = D3DDevice->CreateShaderResourceView(d3d11Tex.Handle, &desc, &nonconst_d3d11Tex.ResourceViewHandle);
		ASSERT(!FAILED(hr));

		ImmediateContext->GenerateMips(nonconst_d3d11Tex.ResourceViewHandle);
	}

	ImmediateContext->PSSetShaderResources(0, 1, &d3d11Tex.ResourceViewHandle);
	ImmediateContext->PSSetSamplers(0, 1, &GlobalSamplerLinear);
}

eastl::shared_ptr<class RHIShader> D3D11RHI::CreateShaderFromSource(const eastl::string& inVertexSrc, const eastl::string& inPixelSrc, const VertexInputLayout& inInputLayout, const eastl::string& inVSName, const eastl::string& inPSName)
 {
	 HRESULT hr = S_OK;
	 ID3D11VertexShader* vertexShaderHandle = nullptr;
	 ID3D11PixelShader* pixelShaderHandle = nullptr;
	 ID3D11InputLayout* vertexShaderLayout = nullptr;

	 // Create Vertex Shader
	 {
		 ID3DBlob* vsBlob = NULL;
		 ID3DBlob* errBlob = NULL;

		 D3DCompile2(inVertexSrc.data(), inVertexSrc.size(), inVSName.c_str(), nullptr, nullptr, "VSEntry", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE, 0, 0, nullptr, 0, &vsBlob, &errBlob);

		 if (!ENSURE(!errBlob))
		 {
			 eastl::string errMessage;
			 errMessage.InitialiseToSize(errBlob->GetBufferSize(), '\0');
			 memcpy(errMessage.data(), errBlob->GetBufferPointer(), errBlob->GetBufferSize());
		 }

		 hr = D3DDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &vertexShaderHandle);
		 ASSERT(!FAILED(hr));


		 constexpr int32_t semanticNamesCount = 4;
		 // This is 1 : 1 with VertexInputType!
		 constexpr char* semanticsName[semanticNamesCount] = {
			 "",
			 "POSITION",
			 "NORMAL",
			 "TEXCOORD"
		 };
		 static_assert(static_cast<int32_t>(VertexInputType::Count) == semanticNamesCount);

		 // Create Vertex Buffer
		 {
			 eastl::vector<D3D11_INPUT_ELEMENT_DESC> elementDescs;

			 int32_t offset = 0;
			 for (const VertexLayoutProperties& prop : inInputLayout.Properties)
			 {
				 D3D11_INPUT_ELEMENT_DESC newDesc = {};

				 DXGI_FORMAT elementFormat = DXGI_FORMAT_UNKNOWN;

				 // TODO: Find a more elegant way
				 if (prop.Type == VertexPropertyType::Float && prop.Count == 3)
				 {
					 elementFormat = DXGI_FORMAT_R32G32B32_FLOAT;
				 }
				 else if (prop.Type == VertexPropertyType::Float && prop.Count == 2)
				 {
					 elementFormat = DXGI_FORMAT_R32G32_FLOAT;
				 }
				 ASSERT(elementFormat != DXGI_FORMAT_UNKNOWN);

				 int32_t semanticIndex = static_cast<int32_t>(prop.InputType);
				 newDesc.SemanticName = semanticsName[semanticIndex];
				 newDesc.Format = elementFormat;
				 newDesc.AlignedByteOffset = offset;
				 newDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

				 offset += prop.Count * prop.GetSizeOfType();

				 elementDescs.push_back(newDesc);
			 }

// 			 D3D11_INPUT_ELEMENT_DESC layout[] =
// 			 {
// 				 { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 				 { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 				 { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
// 			 };
// 			 UINT numElements = ARRAYSIZE(layout);

			 hr = D3DDevice->CreateInputLayout(elementDescs.data(), elementDescs.size(), vsBlob->GetBufferPointer(),
				 vsBlob->GetBufferSize(), &vertexShaderLayout);
			 ASSERT(!FAILED(hr));

			 vsBlob->Release();

			 ImmediateContext->IASetInputLayout(vertexShaderLayout);
		 }
	 }

	 // Create Pixel shader
	 {
		 ID3DBlob* psBlob = NULL;
		 ID3DBlob* psErrBlob = NULL;

		 D3DCompile2(inPixelSrc.data(), inPixelSrc.size(), inPSName.c_str(), nullptr, nullptr, "PSEntry", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE, 0, 0, nullptr, 0, &psBlob, &psErrBlob);

		 if (!ENSURE(!psErrBlob))
		 {
			 eastl::string errMessage;
			 errMessage.InitialiseToSize(psErrBlob->GetBufferSize(), '\0');
			 memcpy(errMessage.data(), psErrBlob->GetBufferPointer(), psErrBlob->GetBufferSize());
		 }
		 hr = D3DDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &pixelShaderHandle);
		 ASSERT(!FAILED(hr));

		 psBlob->Release();
	 }

	 eastl::shared_ptr<D3D11Shader> newShader = eastl::make_shared<D3D11Shader>(vertexShaderHandle, pixelShaderHandle, vertexShaderLayout);

	 return newShader;
 }
 
 eastl::shared_ptr<class RHIShader> D3D11RHI::CreateShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inPixelPath, const VertexInputLayout& inInputLayout)
 {
	 eastl::string fullVertexpath = inVertexPath;
	 fullVertexpath.insert(0, "../Data/Shaders/D3D11/");
	 fullVertexpath.append(".hlsl");

	 eastl::string fullPixelPath = inPixelPath;
	 fullPixelPath.insert(0, "../Data/Shaders/D3D11/");
	 fullPixelPath.append(".hlsl");

	 eastl::string vertexShaderCode;
	 eastl::string fragmentShaderCode;

	 const bool vertexReadSuccess = IOUtils::TryFastReadFile(fullVertexpath, vertexShaderCode);
	 const bool fragmentReadSuccess = IOUtils::TryFastReadFile(fullPixelPath, fragmentShaderCode);
 
 	return CreateShaderFromSource(vertexShaderCode, fragmentShaderCode, inInputLayout);
 }

 void D3D11RHI::ClearBuffers()
 {
	 // Clear the back buffer 
	 ImmediateContext->ClearRenderTargetView(RenderTarget, &CurrentClearColor.x);
 }

 void D3D11RHI::DrawElements(const int32_t inElementsCount)
 {
	 ImmediateContext->DrawIndexed(inElementsCount, 0, 0);
 }

 void D3D11RHI::SwapBuffers()
 {
	 SwapChain->Present(0, 0);
 }

 void D3D11RHI::ClearColor(const glm::vec4 inColor)
 {
	 CurrentClearColor = inColor;
 }

 void D3D11RHI::PrepareProjectionForRendering(glm::mat4& inProj)
 {
	// Default Matrix is D3D11 so don't do anything
 }



void D3D11RHI::BindVertexBuffer(const RHIVertexBuffer& inBuffer, const bool inBindIndexBuffer /*= true*/)
{
	const D3D11VertexBuffer& d3d11Buffer = static_cast<const D3D11VertexBuffer&>(inBuffer);

	uint32_t stride = inBuffer.Layout.Stride;
	uint32_t offset = 0;

	if (inBindIndexBuffer)
	{
		BindIndexBuffer(*(inBuffer.IndexBuffer));
	}

	ImmediateContext->IASetVertexBuffers(0, 1, &d3d11Buffer.Handle, &stride, &offset);
}

void D3D11RHI::BindIndexBuffer(const RHIIndexBuffer& inBuffer)
{
	const D3D11IndexBuffer& d3d11Buffer = static_cast<const D3D11IndexBuffer&>(inBuffer);
	ImmediateContext->IASetIndexBuffer(d3d11Buffer.Handle, DXGI_FORMAT_R32_UINT, 0);
}

void D3D11RHI::BindShader(const RHIShader& inShader)
{
	const D3D11Shader& d3d11Shader = static_cast<const D3D11Shader&>(inShader);

	ImmediateContext->VSSetShader(d3d11Shader.VertexShaderHandle, nullptr, 0);
	ImmediateContext->IASetInputLayout(d3d11Shader.VertexInputLayout);

	ImmediateContext->PSSetShader(d3d11Shader.PixelShaderHandle, nullptr, 0);
}

void D3D11RHI::BindUniformBuffer(const RHIUniformBuffer& inBuffer)
{
	const D3D11UniformBuffer& d3d11Buffer = static_cast<const D3D11UniformBuffer&>(inBuffer);
	ImmediateContext->VSSetConstantBuffers(0, 1, &d3d11Buffer.Handle);

}


void D3D11RHI::UnbindVertexBuffer(const RHIVertexBuffer& inBuffer, const bool inUnbindIndexBuffer /*= true*/)
{
	ImmediateContext->IASetVertexBuffers(0, 0, nullptr, 0, 0);
}

void D3D11RHI::UnbindIndexBuffer(const RHIIndexBuffer& inBuffer)
{
	ImmediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
}

void D3D11RHI::UnbindShader(const RHIShader& inShader)
{
	const D3D11Shader& d3d11Shader = static_cast<const D3D11Shader&>(inShader);

	ImmediateContext->VSSetShader(nullptr, nullptr, 0);
	ImmediateContext->IASetInputLayout(nullptr);

	ImmediateContext->PSSetShader(nullptr, nullptr, 0);
}

void D3D11RHI::UnbindUniformBuffer(const RHIUniformBuffer& inBuffer)
{
	ImmediateContext->VSSetConstantBuffers(0, 0, nullptr);
}

void D3D11RHI::UnbindTexture2D(const RHITexture2D& inTex, const int32_t inTexId)
{
	ImmediateContext->PSSetSamplers(0, 0, nullptr);
	ImmediateContext->PSSetShaderResources(0, 0, nullptr);
}

void D3D11RHI::UniformBufferUpdateData(RHIUniformBuffer& inBuffer, const void* inData, const size_t inDataSize)
{
	D3D11UniformBuffer& d3d11Buffer = static_cast<D3D11UniformBuffer&>(inBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT res = S_OK;
	res = ImmediateContext->Map(d3d11Buffer.Handle, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ASSERT(!FAILED(res));

	memcpy(mappedResource.pData, inData, inDataSize);

	// Unlock the constant buffer.
	ImmediateContext->Unmap(d3d11Buffer.Handle, 0);
}
