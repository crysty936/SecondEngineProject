#include "RHI.h"
#include "Renderer/ForwardRenderer.h"
#include "OpenGL/OpenGLRHI.h"

 #define RENDERERGL 1
 #define RENDERERD3D11 0

//TODO : This holds the current RHI and initializes it when the app starts
// The Forward renderer has to access this and use the rhibase when doing everything and all other specific renderers have to be removed
// Each RHIBase has it's own resource classes that go over the base rhi resource classes

// 
// #if RENDERERGL
// #include "Renderer/OpenGL/OpenGLRenderer.h"
// #elif RENDERERD3D11
// #include "Renderer/D3D11/D3D11Renderer.h"
// #endif 

void RHI::Init()
{
#if RENDERERGL
	Instance = new OpenGLRHI();
#elif RENDERERD3D11
	//TODO
#endif
}

void RHI::Terminate()
{
// #if RENDERERGL
// 	OpenGLRenderer::Terminate();
// #elif RENDERERD3D11
// 	D3D11Renderer::Terminate();
// #endif
}
