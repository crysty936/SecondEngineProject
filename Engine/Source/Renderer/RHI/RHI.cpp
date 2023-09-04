#include "RHI.h"

#define CHOSEN_RENDERER 3

#define RENDERERGL CHOSEN_RENDERER == 1
#define RENDERERD3D11 CHOSEN_RENDERER == 2
#define RENDERERD3D12 CHOSEN_RENDERER == 3

#if RENDERERGL
#include "OpenGL/OpenGLRHI.h"
#elif RENDERERD3D11
#include "D3D11/D3D11RHI.h"
#elif RENDERERD3D12
#include "D3D12/D3D12RHI.h"
#endif

class NullRHI : public RHI
{};

void RHI::Init()
{
#if RENDERERGL
	Instance = new OpenGLRHI();
#elif RENDERERD3D11
	Instance = new D3D11RHI();
#elif RENDERERD3D12
	Instance = new D3D12RHI();
#endif
}

void RHI::Terminate()
{
	delete Get();
}
