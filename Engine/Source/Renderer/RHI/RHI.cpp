#include "RHI.h"
#include "Renderer/TempRenderWrapper.h"
#include "OpenGL/OpenGLRHI.h"

 #define RENDERERGL 1
 #define RENDERERD3D11 0

void RHI::Init()
{
#if RENDERERGL
	Instance = new OpenGLRHI();
#elif RENDERERD3D11
	//TODO
#endif
}

//TODO
void RHI::Terminate()
{
// #if RENDERERGL
// #elif RENDERERD3D11
// #endif
}
