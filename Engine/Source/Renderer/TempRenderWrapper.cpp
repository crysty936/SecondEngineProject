#include "TempRenderWrapper.h"
#include "Window/WindowsWindow.h"

#define RENDERERGL 1
#define RENDERERD3D11 0

#if RENDERERGL
#include "Renderer/OpenGL/OpenGLRenderer.h"
#elif RENDERERD3D11
#include "Renderer/D3D11/D3D11Renderer.h"
#endif 

void TempRenderWrapper::Init()
{
	Instance = new TempRenderWrapper();
#if RENDERERGL
	OpenGLRenderer::Init();
#elif RENDERERD3D11
	D3D11Renderer::Init();
#endif
}

void TempRenderWrapper::Terminate()
{
#if RENDERERGL
	OpenGLRenderer::Terminate();
#elif RENDERERD3D11
	D3D11Renderer::Terminate();
#endif
}

void TempRenderWrapper::Draw()
{
#if RENDERERGL
	OpenGLRenderer::Get().Draw();
#elif RENDERERD3D11
	D3D11Renderer::Get().Draw();
#endif
}

