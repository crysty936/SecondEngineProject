#include "RHI.h"

#define RENDERERINDEX 1

#define RENDERERGL RENDERERINDEX == 1
#define RENDERERD3D11 RENDERERINDEX == 2

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
	delete Get();
}
