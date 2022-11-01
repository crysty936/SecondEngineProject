#include "Renderer/D3D11/D3D11Renderer.h"
#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"

// Windows stuff
#include <windows.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "InputSystem/InputType.h"
#include "Window/WindowsWindow.h"
#include "InputSystem/InputSystem.h"
#include "Utils/IOUtils.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapes.h"

#include <directxmath.h>

D3D_DRIVER_TYPE	g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device* g_pd3dDevice = NULL;
ID3D11DeviceContext* g_pImmediateContext = NULL;
IDXGISwapChain* g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;
ID3D11VertexShader* g_pVertexShader = NULL;
ID3D11PixelShader* g_pPixelShader = NULL;
ID3D11InputLayout* g_pVertexLayout = NULL;
ID3D11Buffer* g_pVertexBuffer = NULL;
ID3D11Buffer* g_pIndexBuffer = NULL;
ID3D11Buffer* g_pConstantBuffer = NULL;

constexpr glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);

eastl::shared_ptr<TriangleShape> triangle = eastl::make_shared<TriangleShape>();

DirectX::XMMATRIX                g_World;
DirectX::XMMATRIX                g_View;
DirectX::XMMATRIX                g_Projection;

struct ConstantBuffer
{
	// 	glm::mat4 mWorld;
	// 	glm::mat4 mView;
	// 	glm::mat4 mProjection;
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

// Works just for ortographic, TODO test for perspective more like the Unreal PrepareProjectionForRHI thing
glm::mat4 mirrorMatrix = {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, -1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
};

glm::mat4 remapZMatrix = {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 0.5f, 0.f,
	0.f, 0.f, 0.5f, 1.f
};

D3D11Renderer::D3D11Renderer(const WindowProperties& inMainWindowProperties)
{
	triangle->Move(glm::vec3(0.f, 5.f, 1.f));


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
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	ASSERT(!FAILED(hr));

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	ASSERT(!FAILED(hr));

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	ASSERT(!FAILED(hr));

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);


	// Create Vertex Shader
	{
		eastl::string vsData;
		bool readSuccess = IOUtils::TryFastReadFile("../Data/Shaders/Direct3D11/ModelWorldPositionVertexShader.hlsl", vsData);
		ASSERT(readSuccess);

		ID3DBlob* vsBlob = NULL;
		ID3DBlob* errBlob = NULL;

		D3DCompile2(vsData.data(), vsData.size(), "../Data/Shaders/Direct3D11/ModelWorldPositionVertexShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE, 0, 0, nullptr, 0, &vsBlob, &errBlob);

		if (!ENSURE(!errBlob))
		{
			eastl::string errMessage;
			errMessage.InitialiseToSize(errBlob->GetBufferSize(), '\0');
			memcpy(errMessage.data(), errBlob->GetBufferPointer(), errBlob->GetBufferSize());
		}

		hr = g_pd3dDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &g_pVertexShader);
		ASSERT(!FAILED(hr));

		// Create Vertex Buffer
		{
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			hr = g_pd3dDevice->CreateInputLayout(layout, numElements, vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(), &g_pVertexLayout);
			ASSERT(!FAILED(hr));

			vsBlob->Release();

			g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
		}
	}

	// Create Pixel shader
	{
		eastl::string psData;
		const bool psReadSuccess = IOUtils::TryFastReadFile("../Data/Shaders/Direct3D11/FlatColorPixelShader.hlsl", psData);
		ASSERT(psReadSuccess);

		ID3DBlob* psBlob = NULL;
		ID3DBlob* psErrBlob = NULL;

		D3DCompile2(psData.data(), psData.size(), "../Data/Shaders/Direct3D11/FlatColorPixelShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_DEBUG_NAME_FOR_SOURCE, 0, 0, nullptr, 0, &psBlob, &psErrBlob);

		if (!ENSURE(!psErrBlob))
		{
			eastl::string errMessage;
			errMessage.InitialiseToSize(psErrBlob->GetBufferSize(), '\0');
			memcpy(errMessage.data(), psErrBlob->GetBufferPointer(), psErrBlob->GetBufferSize());
		}
		hr = g_pd3dDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &g_pPixelShader);
		ASSERT(!FAILED(hr));

		psBlob->Release();
	}

	// Create Index Buffer
	{
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(float) * BasicShapesData::GetCubeVerticesCount();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData = {};
		InitData.pSysMem = BasicShapesData::GetCubeVertices();
		hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
		ASSERT(!FAILED(hr));
	}

	{
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = sizeof(int32_t) * BasicShapesData::GetCubeIndicesCount();
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData = {};
		InitData.pSysMem = BasicShapesData::GetCubeIndices();
		hr = g_pd3dDevice->CreateBuffer(&ibd, &InitData, &g_pIndexBuffer);
		ASSERT(!FAILED(hr));
	}

	{
		D3D11_BUFFER_DESC bd = {};
		// Create the constant buffer
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
		ASSERT(!FAILED(hr));
	}



	// Set vertex buffer
	UINT stride = sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


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

		g_pd3dDevice->CreateRasterizerState(&rastDesc, &newState);
		g_pImmediateContext->RSSetState(newState);
	}

}

D3D11Renderer::~D3D11Renderer() = default;

void D3D11Renderer::Init(const WindowProperties & inMainWindowProperties)
{
	GlobalRHI = new D3D11Renderer{ inMainWindowProperties };


}

void D3D11Renderer::Terminate()
{
	ASSERT(GlobalRHI);
	delete GlobalRHI;
}

void D3D11Renderer::Draw()
{
	// Initialize the world matrix
//g_World = DirectX::XMMatrixTranslation(1.f, 5.f, 20.f);
//g_World = g_World * DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(20.f));

	//glm::mat4 glmWorld = glm::identity<glm::mat4>();
	//glm::mat4 glmWorld = glm::translate(glm::identity<glm::mat4>(), glm::vec3(2.f, 0.f, 10.f));
	//glmWorld = glm::rotate(glmWorld, glm::radians(20.f), glm::vec3(0.f, 0.f, 1.f));
		//triangle->Rotate(20.f, glm::vec3(0.f, 0.f, 1.f));

		//triangle->Rotate(5.f, glm::vec3(0.f, 0.f, 1.f));

	triangle->Rotate(5.f, glm::vec3(0.f, 1.f, 1.f));
	glm::mat4 glmWorld = triangle->GetAbsoluteTransform().GetMatrix();

	memcpy(&g_World, &glmWorld, sizeof(glmWorld));

	glm::mat4 glmView = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.f, 0.f, -5.f));
	glmView = glm::inverse(glmView);

	// Initialize the view matrix
// 	 DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
// 	 DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
// 	 DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
// 	 g_View = DirectX::XMMatrixLookAtLH(Eye, At, Up);
	 //g_View = DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixTranslation(0.f, 0.f, -5.f));

	 // Initialize the projection matrix
	 //g_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width / (FLOAT)height, 0.1f, 1000.0f);


	memcpy(&g_View, &glmView, sizeof(glmView));

	glm::mat4 glmProjection = glm::perspectiveLH_ZO(glm::radians(90.0f), static_cast<float>(Engine->GetMainWindow().GetProperties().Width) / static_cast<float>(Engine->GetMainWindow().GetProperties().Height), 0.1f, 1000.0f);
	//glm::mat4 glmProjection = glm::orthoLH_ZO(-2.f, 2.f, -2.f, 2.f, 0.1f, 1000.f);
	memcpy(&g_Projection, &glmProjection, sizeof(glmProjection));


	// Clear the back buffer 
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);


	//
	// Update variables
	//
	ConstantBuffer cb;
	cb.mWorld = (g_World);
	cb.mView = (g_View);
	cb.mProjection = (g_Projection);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);


	// Render the triangle
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->DrawIndexed(BasicShapesData::GetCubeIndicesCount(), 0, 0);

	// Present the information rendered to the back buffer to the front buffer (the screen)
	g_pSwapChain->Present(0, 0);
}




// 
// void OpenGLRenderer::SetupBaseUniforms()
// {
// 	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(GLWindow->GetProperties().Width) / static_cast<float>(GLWindow->GetProperties().Height), 0.1f, 1000.0f);
// 	UniformsCache["projection"] = projection;
// }
// 
// void OpenGLRenderer::UpdateUniforms()
// {
// 	const glm::mat4 view = SceneManager::Get().GetCurrentScene().CurrentCamera->GetLookAt();
// 	UniformsCache["view"] = view;
// }
// 
