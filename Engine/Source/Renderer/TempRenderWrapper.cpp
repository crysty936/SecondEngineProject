#include "TempRenderWrapper.h"
#include "Window/WindowsWindow.h"

#define RENDERERFORWARD 1
#define RENDERERD3D11 0

#if RENDERERFORWARD
#include "Renderer/ForwardRenderer.h"
#elif RENDERERD3D11
#include "Renderer/D3D11/D3D11Renderer.h"
#endif 

void TempRenderWrapper::Init()
{
#if RENDERERFORWARD
	ForwardRenderer::Init();
#elif RENDERERD3D11
	D3D11Renderer::Init();
#endif
}

void TempRenderWrapper::Terminate()
{
#if RENDERERFORWARD
	ForwardRenderer::Terminate();
#elif RENDERERD3D11
	D3D11Renderer::Terminate();
#endif
}

void TempRenderWrapper::Draw()
{
#if RENDERERFORWARD
	ForwardRenderer::Get().Draw();
#elif RENDERERD3D11
	D3D11Renderer::Get().Draw();
#endif
}

