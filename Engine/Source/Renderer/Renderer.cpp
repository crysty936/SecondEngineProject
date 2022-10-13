#include "Renderer.h"
#include "Window/WindowsWindow.h"

#define RENDERERGL 0
#define RENDERERD3D11 1

#if RENDERERGL
#include "Renderer/OpenGL/OpenGLRenderer.h"
#elif RENDERERD3D11
#include "Renderer/D3D11/D3D11Renderer.h"
#endif 

void Renderer::Init()
{
#if RENDERERGL
	OpenGLRenderer::Init();
#elif RENDERERD3D11
	D3D11Renderer::Init();
#endif
}

void Renderer::Terminate()
{
#if RENDERERGL
	OpenGLRenderer::Terminate();
#elif RENDERERD3D11
	D3D11Renderer::Terminate();
#endif

}

void Renderer::Draw()
{
#if RENDERERGL
	OpenGLRenderer::GetRHI().Draw();
#elif RENDERERD3D11
	D3D11Renderer::GetRHI().Draw();
#endif
}

WindowsWindow& Renderer::GetMainWindow()
{
#if RENDERERGL
	return OpenGLRenderer::GetRHI().GetMainWindow();
#elif RENDERERD3D11
	return D3D11Renderer::GetRHI().GetMainWindow();
#endif
}
