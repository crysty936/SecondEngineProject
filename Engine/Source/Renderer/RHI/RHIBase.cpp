#include "RHIBase.h"
#include "Renderer/ForwardRenderer.h"
#include "OpenGL/OpenGLRHI.h"

 #define RENDERERGL 1
 #define RENDERERD3D11 0
// 
// #if RENDERERGL
// #include "Renderer/OpenGL/OpenGLRenderer.h"
// #elif RENDERERD3D11
// #include "Renderer/D3D11/D3D11Renderer.h"
// #endif 

void RHIBase::Init()
{
#if RENDERERGL
	RHI = new OpenGLRHI();
#elif RENDERERD3D11
	//TODO
#endif
}

void RHIBase::Terminate()
{
// #if RENDERERGL
// 	OpenGLRenderer::Terminate();
// #elif RENDERERD3D11
// 	D3D11Renderer::Terminate();
// #endif
}
