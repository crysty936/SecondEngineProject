#include "RHI.h"
#include "Renderer/TempRenderWrapper.h"

 #define RENDERERGL 0
 #define RENDERERD3D11 1

#if RENDERERGL
#include "OpenGL/OpenGLRHI.h"
#elif RENDERERD3D11
#include "D3D11/D3D11RHI.h"
#endif

void RHI::Init()
{
#if RENDERERGL
	Instance = new OpenGLRHI();
#elif RENDERERD3D11
	Instance = new D3D11RHI();
#endif
}

void RHI::Terminate()
{
	delete Instance;
}
