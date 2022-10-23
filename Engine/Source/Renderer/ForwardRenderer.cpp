#include "ForwardRenderer.h"
#include "Window/WindowsWindow.h"

#define RENDERERGL 1
#define RENDERERD3D11 0

#if RENDERERGL
#include "Renderer/OpenGL/OpenGLRenderer.h"
#elif RENDERERD3D11
#include "Renderer/D3D11/D3D11Renderer.h"
#endif 

void ForwardRenderer::Init()
{
	Instance = new ForwardRenderer();
#if RENDERERGL
	OpenGLRenderer::Init();
#elif RENDERERD3D11
	D3D11Renderer::Init();
#endif
}

void ForwardRenderer::Terminate()
{
#if RENDERERGL
	OpenGLRenderer::Terminate();
#elif RENDERERD3D11
	D3D11Renderer::Terminate();
#endif
}

void ForwardRenderer::Draw()
{
#if RENDERERGL
	OpenGLRenderer::GetRHI().Draw();
#elif RENDERERD3D11
	D3D11Renderer::GetRHI().Draw();
#endif
}

