#include "D3D12RHI.h"
#include <windows.h>

#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"
#include "Window/WindowsWindow.h"
#include "Utils/IOUtils.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <DirectXMath.h>
//#include "glm/ext/vector_common.inl"
#include "glm/glm.hpp"


#include "DirectXTex.h"
#include "Utils/ImageLoading.h"


using Microsoft::WRL::ComPtr;


inline eastl::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return eastl::string(s_str);
}

void GetHardwareAdapter(
	IDXGIFactory1* pFactory,
	IDXGIAdapter1** ppAdapter,
	bool requestHighPerformanceAdapter)
{
	*ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapter;

	ComPtr<IDXGIFactory6> factory6;
	if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
	{
		for (
			UINT adapterIndex = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(
				adapterIndex,
				requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
				IID_PPV_ARGS(&adapter)));
			++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see whether the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}
	}

	if (adapter.Get() == nullptr)
	{
		for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see whether the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}
	}

	*ppAdapter = adapter.Detach();
}


inline bool DXAssert(HRESULT inRez)
{
	const bool success = SUCCEEDED(inRez);
	ASSERT_MSG(success, "Direct3D12 Operation failed with code 0x%08X", static_cast<uint32_t>(inRez));

	return success;
}

static const UINT FrameCount = 2;

struct TestVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 UV;
};

static const UINT TextureWidth = 256;
static const UINT TextureHeight = 256;
static const UINT TexturePixelSize = 4;

// Pipeline objects.
D3D12_VIEWPORT m_viewport;
D3D12_RECT m_scissorRect;
ComPtr<IDXGISwapChain3> m_swapChain;
ComPtr<ID3D12Device> m_device;
ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
ComPtr<ID3D12CommandAllocator> m_commandAllocators[FrameCount];
ComPtr<ID3D12CommandQueue> m_commandQueue;
ComPtr<ID3D12RootSignature> m_rootSignature;
ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
ComPtr<ID3D12DescriptorHeap> m_srvHeap;
ComPtr<ID3D12PipelineState> m_pipelineState;
ComPtr<ID3D12GraphicsCommandList> m_commandList;
UINT m_rtvDescriptorSize;
UINT m_srvDescriptorSize;

// App resources.
ComPtr<ID3D12Resource> m_vertexBuffer;
D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
ComPtr<ID3D12Resource> m_texture;


// Constant Buffer
struct SceneConstantBuffer
{
	float offset;
	float padding[63]; // Padding so the constant buffer is 256-byte aligned.
};
static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
ComPtr<ID3D12Resource> m_constantBuffer;
SceneConstantBuffer m_constantBufferData;
UINT8* m_pCbvDataBegin;

// Synchronization objects.
UINT m_frameIndex = 0;
HANDLE m_fenceEvent;
ComPtr<ID3D12Fence> m_fence;

#define FRAME_BUFFERING 0

#if FRAME_BUFFERING
UINT64 m_fenceValues[FrameCount];
#else
UINT64 m_fenceValue;
#endif

void InitPipeline()
{
	UINT dxgiFactoryFlags = 0;

//#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
//#endif

	ComPtr<IDXGIFactory4> factory;
	DXAssert(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

	ComPtr<IDXGIAdapter1> hardwareAdapter;
	GetHardwareAdapter(factory.Get(), &hardwareAdapter, false);

	DXAssert(D3D12CreateDevice(
		hardwareAdapter.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_device)
	));


	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	DXAssert(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

	const WindowsWindow& mainWindow = GEngine->GetMainWindow();
	const WindowProperties& props = mainWindow.GetProperties();
	m_viewport.Width = static_cast<float>(props.Width);
	m_viewport.Height = static_cast<float>(props.Height);

	m_scissorRect.left = 0;
	m_scissorRect.top = 0;
	m_scissorRect.right = props.Width;
	m_scissorRect.bottom = props.Height;

	// Describe and create the swap chain.
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = props.Width;
	swapChainDesc.Height = props.Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;


	ComPtr<IDXGISwapChain1> swapChain;
	DXAssert(factory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
		static_cast<HWND>(mainWindow.GetHandle()),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));

	DXAssert(swapChain.As(&m_swapChain));


	// Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = FrameCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DXAssert(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

		m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Describe and create a constant buffer view (CBV) descriptor heap.
		// Flags indicate that this descriptor heap can be bound to the pipeline 
		// and that descriptors contained in it can be referenced by a root table.
		//D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
		//cbvHeapDesc.NumDescriptors = 1;
		//cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		//cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		//DXAssert(m_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeap)));

		// Describe and create a shader resource view (SRV) heap for the texture.
		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 2;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DXAssert(m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));

		m_srvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	}


	// Create frame resources.
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < FrameCount; n++)
		{
			DXAssert(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);

			rtvHandle.ptr += size_t(m_rtvDescriptorSize);
			DXAssert(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n])));
		}
	}

}


// Generate a simple black and white checkerboard texture.
std::vector<UINT8> GenerateTextureData()
{
	const UINT rowPitch = TextureWidth * TexturePixelSize;
	const UINT cellPitch = rowPitch >> 3;        // The width of a cell in the checkboard texture.
	const UINT cellHeight = TextureWidth >> 3;    // The height of a cell in the checkerboard texture.
	const UINT textureSize = rowPitch * TextureHeight;

	std::vector<UINT8> data(textureSize);
	UINT8* pData = &data[0];

	for (UINT n = 0; n < textureSize; n += TexturePixelSize)
	{
		UINT x = n % rowPitch;
		UINT y = n / rowPitch;
		UINT i = x / cellPitch;
		UINT j = y / cellHeight;

		if (i % 2 == j % 2)
		{
			pData[n] = 0x00;        // R
			pData[n + 1] = 0x00;    // G
			pData[n + 2] = 0x00;    // B
			pData[n + 3] = 0xff;    // A
		}
		else
		{
			pData[n] = 0xff;        // R
			pData[n + 1] = 0xff;    // G
			pData[n + 2] = 0xff;    // B
			pData[n + 3] = 0xff;    // A
		}
	}

	return data;
}

// Returns required size of a buffer to be used for data upload
inline UINT64 GetRequiredIntermediateSize(
	ID3D12Resource* pDestinationResource,
	UINT FirstSubresource,
	UINT NumSubresources) noexcept
{
	const D3D12_RESOURCE_DESC Desc = pDestinationResource->GetDesc();
	UINT64 RequiredSize = 0;

	ID3D12Device* device = nullptr;
	pDestinationResource->GetDevice(_uuidof(ID3D12Device), reinterpret_cast<void**>(&device));
	device->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);
	device->Release();

	return RequiredSize;
}

void DoTheUploadBlocking(ID3D12Resource* inDestResource, ID3D12Resource* inUploadResource, const D3D12_SUBRESOURCE_DATA* inSrcData, uint32_t NumSubresources, ID3D12GraphicsCommandList* inCmdList, DirectX::ScratchImage& inRes);


D3D12RHI::D3D12RHI()
{
	InitPipeline();


	// Load Assets

	// Create a root signature consisting of a descriptor table with a single CBV.
	{
		D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

		// This is the highest version the sample supports. If CheckFeatureSupport succeeds, the HighestVersion returned will not be greater than this.
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

		if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
		{
			featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
		}

		//////////////////////////////////////////////////////////////////////////

		//D3D12_DESCRIPTOR_RANGE1 ranges[2];

// 		// Constant Buffer
// 		ranges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
// 		ranges[1].NumDescriptors = 1;
// 		ranges[1].BaseShaderRegister = 0;
// 		ranges[1].RegisterSpace = 0;
// 		ranges[1].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
// 		ranges[1].OffsetInDescriptorsFromTableStart = 0;
// 
// 		// Texture
// 		ranges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
// 		ranges[0].NumDescriptors = 1;
// 		ranges[0].BaseShaderRegister = 0;
// 		ranges[0].RegisterSpace = 0;
// 		ranges[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
// 		ranges[0].OffsetInDescriptorsFromTableStart = 1;
// 
// 
// 		D3D12_ROOT_PARAMETER1 rootParameters[1];
// 		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
// 		//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
// 		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
// 		rootParameters[0].DescriptorTable.NumDescriptorRanges = 2;
// 		rootParameters[0].DescriptorTable.pDescriptorRanges = &ranges[0];
		

		//////////////////////////////////////////////////////////////////////////

 		D3D12_DESCRIPTOR_RANGE1 rangesVS[2];
 
 		// Constant Buffer
 		rangesVS[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
 		rangesVS[0].NumDescriptors = 1;
 		rangesVS[0].BaseShaderRegister = 1;
 		rangesVS[0].RegisterSpace = 0;
 		rangesVS[0].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
 		rangesVS[0].OffsetInDescriptorsFromTableStart = 0;
 
 		//D3D12_DESCRIPTOR_RANGE1 rangesPS[1];
 		// Texture
 		rangesVS[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
 		rangesVS[1].NumDescriptors = 1;
 		rangesVS[1].BaseShaderRegister = 0;
 		rangesVS[1].RegisterSpace = 0;
 		rangesVS[1].Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC;
 		rangesVS[1].OffsetInDescriptorsFromTableStart = 1;
 
 		D3D12_ROOT_PARAMETER1 rootParameters[2];
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
		rootParameters[0].Constants.Num32BitValues = 4;
		rootParameters[0].Constants.RegisterSpace = 0;
		rootParameters[0].Constants.ShaderRegister = 0;

 		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
 		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
 		rootParameters[1].DescriptorTable.NumDescriptorRanges = 2;
 		rootParameters[1].DescriptorTable.pDescriptorRanges = &rangesVS[0];
 
 		//rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
 		//rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
 		//rootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
 		//rootParameters[2].DescriptorTable.pDescriptorRanges = &rangesPS[0];


		//////////////////////////////////////////////////////////////////////////

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		// Allow input layout and deny uneccessary access to certain pipeline stages.
		D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
			| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
			//| D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Version = D3D_ROOT_SIGNATURE_VERSION_1_1;
		rootSignatureDesc.Desc_1_1.NumParameters = _countof(rootParameters);
		rootSignatureDesc.Desc_1_1.pParameters = &rootParameters[0];
		rootSignatureDesc.Desc_1_1.NumStaticSamplers = 1;
		rootSignatureDesc.Desc_1_1.pStaticSamplers = &sampler;
		rootSignatureDesc.Desc_1_1.Flags = rootSignatureFlags;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		if (!DXAssert(D3D12SerializeVersionedRootSignature(&rootSignatureDesc, &signature, &error)))
		{
			const char* errText = (char*)error->GetBufferPointer();
			LOG_ERROR("%s", errText);
		}


		DXAssert(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
	}
	
	eastl::string fullPath = "shaders";
	fullPath.insert(0, "../Data/Shaders/D3D12/");
	fullPath.append(".hlsl");

#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE;
#else																						  
	UINT compileFlags = 0;
#endif

	eastl::string shaderCode;
	const bool readSuccess = IOUtils::TryFastReadFile(fullPath, shaderCode);



	ComPtr<ID3DBlob> vertexShader;
	ID3DBlob* vsErrBlob = nullptr;


	D3DCompile2(shaderCode.data(), shaderCode.size(), "testshadername", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, 0, nullptr, 0, &vertexShader, &vsErrBlob);


	if (!ENSURE(!vsErrBlob))
	{
		eastl::string errMessage;
		errMessage.InitialiseToSize(vsErrBlob->GetBufferSize(), '\0');
		memcpy(errMessage.data(), vsErrBlob->GetBufferPointer(), vsErrBlob->GetBufferSize());
		LOG_ERROR("%s", errMessage.c_str());
	}

	ComPtr<ID3DBlob> pixelShader;
	ID3DBlob* psErrBlob = nullptr;

	D3DCompile2(shaderCode.data(), shaderCode.size(), "testshadername", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, 0, nullptr, 0, &pixelShader, &psErrBlob);

	if (!ENSURE(!psErrBlob))
	{
		eastl::string errMessage;
		errMessage.InitialiseToSize(psErrBlob->GetBufferSize(), '\0');
		memcpy(errMessage.data(), psErrBlob->GetBufferPointer(), psErrBlob->GetBufferSize());
		LOG_ERROR("%s", errMessage.c_str());
	}


	// Define the vertex input layout.
		// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// shader bytecodes
	D3D12_SHADER_BYTECODE vsByteCode;
	vsByteCode.pShaderBytecode = vertexShader->GetBufferPointer();
	vsByteCode.BytecodeLength = vertexShader->GetBufferSize();

	D3D12_SHADER_BYTECODE psByteCode;
	psByteCode.pShaderBytecode = pixelShader->GetBufferPointer();
	psByteCode.BytecodeLength = pixelShader->GetBufferSize();

	// Rasterizer State
	D3D12_RASTERIZER_DESC rastState;
	rastState.FillMode = D3D12_FILL_MODE_SOLID;
	rastState.CullMode = D3D12_CULL_MODE_BACK;
	rastState.FrontCounterClockwise = FALSE;
	rastState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rastState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rastState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rastState.DepthClipEnable = TRUE;
	rastState.MultisampleEnable = FALSE;
	rastState.AntialiasedLineEnable = FALSE;
	rastState.ForcedSampleCount = 0;
	rastState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	// Blend State

	D3D12_BLEND_DESC blendState;
	blendState.AlphaToCoverageEnable = FALSE;
	blendState.IndependentBlendEnable = FALSE;

	const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		FALSE,FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL,
	};

	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		blendState.RenderTarget[i] = defaultRenderTargetBlendDesc;

	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = vsByteCode;
	psoDesc.PS = psByteCode;
	psoDesc.RasterizerState = rastState;
	psoDesc.BlendState = blendState;
	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	DXAssert(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));

	// Create the command list.
	DXAssert(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get(), IID_PPV_ARGS(&m_commandList)));

	const WindowsWindow& mainWindow = GEngine->GetMainWindow();
	const WindowProperties& props = mainWindow.GetProperties();

	const int32_t aspectRatio = props.Width / props.Height;

	// Create the vertex buffer.
	{
		// Define the geometry for a triangle.
		TestVertex triangleVertices[] =
		{
			{ { 0.0f, 0.25f * aspectRatio, 0.0f }, { 0.5f, 0.0f } },
			{ { 0.25f, -0.25f * aspectRatio, 0.0f }, { 1.0f, 1.0f } },
			{ { -0.25f, -0.25f * aspectRatio, 0.0f }, { 0.0f, 1.0f } }
		};

		const UINT vertexBufferSize = sizeof(triangleVertices);

		// Note: using upload heaps to transfer static data like vert buffers is not 
		// recommended. Every time the GPU needs it, the upload heap will be marshalled 
		// over. Please read up on Default Heap usage. An upload heap is used here for 
		// code simplicity and because there are very few verts to actually transfer.
		D3D12_HEAP_PROPERTIES heapProps;
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.CreationNodeMask = 1;
		heapProps.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC vertexBufferDesc;
		vertexBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexBufferDesc.Alignment = 0;
		vertexBufferDesc.Width = vertexBufferSize;
		vertexBufferDesc.Height = 1;
		vertexBufferDesc.DepthOrArraySize = 1;
		vertexBufferDesc.MipLevels = 1;
		vertexBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
		vertexBufferDesc.SampleDesc.Count = 1;
		vertexBufferDesc.SampleDesc.Quality = 0;
		vertexBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vertexBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;


		DXAssert(m_device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&vertexBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_vertexBuffer)));

		// Copy the triangle data to the vertex buffer.
		UINT8* pVertexDataBegin;
		D3D12_RANGE readRange;
		readRange.Begin = 0;
		readRange.End = 0;
		DXAssert(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
		memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
		m_vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes = sizeof(TestVertex);
		m_vertexBufferView.SizeInBytes = vertexBufferSize;
	}


	// Create the constant buffer.
	{
		const UINT constantBufferSize = sizeof(SceneConstantBuffer);    // CB size is required to be 256-byte aligned.

		D3D12_HEAP_PROPERTIES heapProps;
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.CreationNodeMask = 1;
		heapProps.VisibleNodeMask = 1;


		D3D12_RESOURCE_DESC constantBufferDesc;
		constantBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		constantBufferDesc.Alignment = 0;
		constantBufferDesc.Width = constantBufferSize;
		constantBufferDesc.Height = 1;
		constantBufferDesc.DepthOrArraySize = 1;
		constantBufferDesc.MipLevels = 1;
		constantBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
		constantBufferDesc.SampleDesc.Count = 1;
		constantBufferDesc.SampleDesc.Quality = 0;
		constantBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		constantBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		DXAssert(m_device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&constantBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_constantBuffer)));

		// Describe and create a constant buffer view.
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBufferSize;
		m_device->CreateConstantBufferView(&cbvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());


		// Map and initialize the constant buffer. We don't unmap this until the
		// app closes. Keeping things mapped for the lifetime of the resource is okay.
		D3D12_RANGE readRange;
		readRange.Begin = 0;
		readRange.End = 0;      // We do not intend to read from this resource on the CPU.
		DXAssert(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
		memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
	}


	// Note: ComPtr's are CPU objects but this resource needs to stay in scope until
	// the command list that references it has finished executing on the GPU.
	// We will flush the GPU at the end of this method to ensure the resource is not
	// prematurely destroyed.
	ComPtr<ID3D12Resource> textureUploadHeap;

	CreateTextureStuff(textureUploadHeap.Get());

	// Create synchronization objects and wait until assets have been uploaded to the GPU.
	{
#if FRAME_BUFFERING
		m_fenceValues[0] = m_fenceValues[1] = 1;

		DXAssert(m_device->CreateFence(m_fenceValues[0], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fenceValues[0]++;
#else
		DXAssert(m_device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
		m_fenceValue++;
#endif

		// Create an event handle to use for frame synchronization.
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (m_fenceEvent == nullptr)
		{
			DXAssert(HRESULT_FROM_WIN32(GetLastError()));
		}

		// Wait for the command list to execute; we are reusing the same command 
		// list in our main loop but for now, we just want to wait for setup to 
		// complete before continuing.
		WaitForPreviousFrame();
	}
}


void DoTheUploadBlocking(ID3D12Resource* inDestResource, ID3D12Resource* inUploadResource, const D3D12_SUBRESOURCE_DATA* inSrcData, uint32_t NumSubresources, ID3D12GraphicsCommandList* inCmdList, DirectX::ScratchImage& inRes)
{
	const uint32_t numMips = inRes.GetImageCount();
	const DirectX::TexMetadata& metaData = inRes.GetMetadata();
	NumSubresources = numMips;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT* layouts = static_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(_alloca(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) * NumSubresources));
	ASSERT(layouts != nullptr);

	uint64_t* rowSize = static_cast<uint64_t*>(_alloca(sizeof(uint64_t) * NumSubresources));
	ASSERT(rowSize != nullptr);

	uint32_t* numRows = static_cast<uint32_t*>(_alloca(sizeof(uint32_t) * NumSubresources));
	ASSERT(numRows != nullptr);

	uint64_t requiredSize = 0;

	const D3D12_RESOURCE_DESC DestDesc = inDestResource->GetDesc();
	ID3D12Device* device = nullptr;
	inDestResource->GetDevice(_uuidof(ID3D12Device), reinterpret_cast<void**>(&device));

	constexpr uint32_t firstSubresource = 0;
	device->GetCopyableFootprints(&DestDesc, 0, NumSubresources, 0, layouts, numRows, rowSize, &requiredSize);
	device->Release();

	const D3D12_RESOURCE_DESC uploadBufferDesc = inUploadResource->GetDesc();

	const bool isCopyValid =
		uploadBufferDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
		uploadBufferDesc.Width >= requiredSize + layouts[0].Offset &&
		requiredSize <= size_t(-1) &&
		(DestDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER || (NumSubresources == 1));

	ASSERT(isCopyValid);

	char* uploadResourceCPUMem = nullptr;

	HRESULT hr = inUploadResource->Map(0, nullptr, reinterpret_cast<void**>(&uploadResourceCPUMem));

	uint8_t* uploadMem = reinterpret_cast<uint8_t*>(uploadResourceCPUMem);

	if (!ASSERT(SUCCEEDED(hr)))
	{
		return;
	}

	//const uint64_t srcTexelSize = DirectX::BitsPerPixel(metaData.format);
	//uint64_t mipWidth = metaData.width;

	// Copy to the staging upload heap
	for (uint32_t i = 0; i < numMips; ++i)
	{
		ASSERT(rowSize[i] <= size_t(-1));

		const uint64_t mipId = i;

		const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& mipSubresourceLayout = layouts[mipId];
		const uint64_t subResourceHeight = numRows[mipId];
		const uint64_t subResourcePitch = mipSubresourceLayout.Footprint.RowPitch;
		const uint64_t subResourceDepth = mipSubresourceLayout.Footprint.Depth;
		//const uint64_t srcPitch = mipWidth * srcTexelSize;

		uint8_t* dstSubresourceMem = uploadMem + mipSubresourceLayout.Offset;

		for (uint64_t z = 0; z < subResourceDepth; ++z)
		{
			const DirectX::Image* currentSubImage = inRes.GetImage(i, 0, z);
			ASSERT(currentSubImage != nullptr);
			const uint8_t* srcSubImageTexels = currentSubImage->pixels;

			for (uint64_t y = 0; y < subResourceHeight; ++y)
			{
				const uint8_t* accessTexels = srcSubImageTexels + glm::min(subResourcePitch, currentSubImage->rowPitch);
				uint8_t accessTexel = *accessTexels;
				memcpy(dstSubresourceMem, srcSubImageTexels, glm::min(subResourcePitch, currentSubImage->rowPitch));
				dstSubresourceMem += subResourcePitch;
				srcSubImageTexels += currentSubImage->rowPitch;
			}



		}
		//mipWidth = glm::max(mipWidth / 2, 1ull);


	}

	inUploadResource->Unmap(0, nullptr);


	if (DestDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
	{
		inCmdList->CopyBufferRegion(inDestResource, 0, inUploadResource, layouts[0].Offset, layouts[0].Footprint.Width);
	}
	else
	{
		for (uint32_t i = 0; i < NumSubresources; ++i)
		{
			D3D12_TEXTURE_COPY_LOCATION dest = {};
			dest.pResource = inDestResource;
			dest.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dest.PlacedFootprint = {};
			dest.SubresourceIndex = i + firstSubresource;


			D3D12_TEXTURE_COPY_LOCATION src = {};
			src.pResource = inUploadResource;
			src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			src.PlacedFootprint = layouts[i];

			inCmdList->CopyTextureRegion(&dest, 0, 0, 0, &src, nullptr);
		}
	}
}

void D3D12RHI::CreateTextureStuff(ID3D12Resource* inUploadHeap)
{
	// Create the texture.
	{
		D3D12_HEAP_PROPERTIES DefaultHeapProps;
		DefaultHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
		DefaultHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		DefaultHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		DefaultHeapProps.CreationNodeMask = 1;
		DefaultHeapProps.VisibleNodeMask = 1;

		D3D12_HEAP_PROPERTIES UploadHeapProps;
		UploadHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		UploadHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		UploadHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		UploadHeapProps.CreationNodeMask = 1;
		UploadHeapProps.VisibleNodeMask = 1;


		DirectX::ScratchImage dxImageTest = {};
		DirectX::TexMetadata dxMetadataTest = {};

		HRESULT hresult = DirectX::LoadFromWICFile(L"../Data/Textures/MinecraftGrass.jpg", DirectX::WIC_FLAGS_NONE, &dxMetadataTest, dxImageTest);
		const bool success = SUCCEEDED(hresult);

		DirectX::ScratchImage res;
		DirectX::GenerateMipMaps(*dxImageTest.GetImage(0, 0, 0), DirectX::TEX_FILTER_FLAGS::TEX_FILTER_DEFAULT, 0, res, false);


		// Describe and create a Texture2D on GPU(Default Heap)
		D3D12_RESOURCE_DESC textureDesc = {};
		textureDesc.MipLevels = res.GetImageCount();
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.Width = dxMetadataTest.width;
		textureDesc.Height = dxMetadataTest.height;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

		DXAssert(m_device->CreateCommittedResource(
			&DefaultHeapProps,
			D3D12_HEAP_FLAG_NONE,
			&textureDesc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&m_texture)));


		// Get required size by device
		UINT64 uploadBufferSize = 0;
		m_device->GetCopyableFootprints(&textureDesc, 0, res.GetImageCount(), 0, nullptr, nullptr, nullptr, &uploadBufferSize);

		// Same thing
		//const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);

		D3D12_RESOURCE_DESC UploadBufferDesc;
		UploadBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		UploadBufferDesc.Alignment = 0;
		UploadBufferDesc.Width = uploadBufferSize;
		UploadBufferDesc.Height = 1;
		UploadBufferDesc.DepthOrArraySize = 1;
		UploadBufferDesc.MipLevels = 1;
		UploadBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
		UploadBufferDesc.SampleDesc.Count = 1;
		UploadBufferDesc.SampleDesc.Quality = 0;
		UploadBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		UploadBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		// Create the CPU -> GPU Upload Buffer
		DXAssert(m_device->CreateCommittedResource(
			&UploadHeapProps,
			D3D12_HEAP_FLAG_NONE,
			&UploadBufferDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&inUploadHeap)));


		// Copy data to the intermediate upload heap and then schedule a copy 
		// from the upload heap to the Texture2D.
		//std::vector<UINT8> texture = GenerateTextureData();

		D3D12_SUBRESOURCE_DATA textureData = {};
		//textureData.pData = res.GetPixels();
		//textureData.RowPitch = res.GetPixelsSize();
		//textureData.RowPitch = TextureWidth * TexturePixelSize;
		//textureData.SlicePitch = textureData.RowPitch * data.Height;
		//textureData.SlicePitch = 1;


		// Blocking call
		DoTheUploadBlocking(m_texture.Get(), inUploadHeap, &textureData, 1, m_commandList.Get(), res);

		D3D12_RESOURCE_BARRIER barrier;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_texture.Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		m_commandList->ResourceBarrier(1, &barrier);

		// Describe and create a SRV for the texture.
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = res.GetImageCount();
		//srvDesc.Texture2D.MipLevels = 1;


		D3D12_CPU_DESCRIPTOR_HANDLE descHeapStartHandle = m_srvHeap->GetCPUDescriptorHandleForHeapStart();
		descHeapStartHandle.ptr += 1 * m_srvDescriptorSize;
		m_device->CreateShaderResourceView(m_texture.Get(), &srvDesc, descHeapStartHandle);
	}

	DXAssert(m_commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}



D3D12RHI::~D3D12RHI()
{
	WaitForPreviousFrame();

	CloseHandle(m_fenceEvent);
}

#if FRAME_BUFFERING
void D3D12RHI::WaitForPreviousFrame()
{
	const UINT64 currentFrameRequiredFenceValue = m_fenceValues[m_frameIndex];
	// We want that fence to be set to that value from the GPU side
	DXAssert(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_frameIndex]));

	// Tell m_fence to raise this event once it's equal fence value
	DXAssert(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));

	// Wait until that event is raised
	WaitForSingleObject(m_fenceEvent, INFINITE);

	m_fenceValues[m_frameIndex]++;
}
#else
void D3D12RHI::WaitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
	// sample illustrates how to use fences for efficient resource usage and to
	// maximize GPU utilization.


	// Basically tells the GPU that it should set that fence to that value.
	// Because of the way the queue works, that command is only going to be executed once all other command lists in the queue are done

	// Signal and increment the fence value.
	const UINT64 fence = m_fenceValue;
	// We want that fence to be set to that value from the GPU side
	DXAssert(m_commandQueue->Signal(m_fence.Get(), fence));
	m_fenceValue++;

	const UINT64 fenceValue = m_fence->GetCompletedValue();
	if (fenceValue < fence)
	{
		// Tell m_fence to raise this event once it's equal fence value
		DXAssert(m_fence->SetEventOnCompletion(fence, m_fenceEvent));

		// Wait until that event is raised
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}
#endif


#if FRAME_BUFFERING
// Prepare to render the next frame.
void D3D12RHI::MoveToNextFrame()
{
	const UINT64 submittedFrameFenceValue = m_fenceValues[m_frameIndex];

	// Place signal for frame that was just submitted
	DXAssert(m_commandQueue->Signal(m_fence.Get(), submittedFrameFenceValue));

	// Move onto next frame. Backbuffer index was changed as Present was called before this
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	const UINT64 presentFenceValue = m_fence->GetCompletedValue();

	// Happens if Graphics device is removed while running
	if (presentFenceValue == UINT64_MAX)
	{
		__debugbreak();
	}

	const UINT64 toStartFrameFenceValue = m_fenceValues[m_frameIndex];

	if (presentFenceValue < toStartFrameFenceValue)
	{
		DXAssert(m_fence->SetEventOnCompletion(toStartFrameFenceValue, m_fenceEvent));
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, false);
	}

	m_fenceValues[m_frameIndex] = submittedFrameFenceValue + 1;
}
#endif

void D3D12RHI::Test()
{
	const float translationSpeed = 0.005f;
	const float offsetBounds = 1.25f;

	float offset = m_constantBufferData.offset;
	offset += translationSpeed;
	if (offset > offsetBounds)
	{
		offset = -offsetBounds;
	}
	m_constantBufferData.offset = offset;

	memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));

	// Populate Command List

	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	DXAssert(m_commandAllocators[m_frameIndex]->Reset());

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	DXAssert(m_commandList->Reset(m_commandAllocators[m_frameIndex].Get(), m_pipelineState.Get()));

	m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	// Constant buffer
// 	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvHeap.Get() };
// 	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
// 	m_commandList->SetGraphicsRootDescriptorTable(0, m_cbvHeap->GetGPUDescriptorHandleForHeapStart());

	// Desc Heap
	ID3D12DescriptorHeap* ppHeaps[] = { m_srvHeap.Get() };
	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// First table
	glm::vec4 rootConstant(0.f);
	rootConstant.x = -offset;
	m_commandList->SetGraphicsRoot32BitConstants(0, 4, &rootConstant, 0);

	m_commandList->SetGraphicsRootDescriptorTable(1, m_srvHeap->GetGPUDescriptorHandleForHeapStart());

	// Second table
	//D3D12_GPU_DESCRIPTOR_HANDLE secondTableHandle(m_srvHeap->GetGPUDescriptorHandleForHeapStart());
	//secondTableHandle.ptr += size_t(m_srvDescriptorSize);
	//m_commandList->SetGraphicsRootDescriptorTable(2, secondTableHandle); // This does the magic of binding a certain descriptor table to a certain heap with a start index for accessing descriptors


	D3D12_RESOURCE_BARRIER transitionPresentToRt;
	transitionPresentToRt.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	transitionPresentToRt.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	transitionPresentToRt.Transition.pResource = m_renderTargets[m_frameIndex].Get();
	transitionPresentToRt.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	transitionPresentToRt.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	transitionPresentToRt.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	// Indicate that the back buffer will be used as a render target.
	m_commandList->ResourceBarrier(1, &transitionPresentToRt);


	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	rtvHandle.ptr = m_rtvHeap->GetCPUDescriptorHandleForHeapStart().ptr + m_frameIndex * m_rtvDescriptorSize;

	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
	m_commandList->DrawInstanced(3, 1, 0, 0);


	D3D12_RESOURCE_BARRIER transitionRtToPresent;
	transitionRtToPresent.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	transitionRtToPresent.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	transitionRtToPresent.Transition.pResource = m_renderTargets[m_frameIndex].Get();
	transitionRtToPresent.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	transitionRtToPresent.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	transitionRtToPresent.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;


	// Indicate that the back buffer will now be used to present.
	m_commandList->ResourceBarrier(1, &transitionRtToPresent);

	DXAssert(m_commandList->Close());

	ID3D12CommandList* commandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(1, commandLists);

	m_swapChain->Present(1, 0);

#if FRAME_BUFFERING
	MoveToNextFrame();
#else
	WaitForPreviousFrame();
#endif

}
