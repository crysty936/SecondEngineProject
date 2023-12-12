#include "RHI.h"

#define CHOSEN_API 3

#define API_GL CHOSEN_API == 1
#define API_D3D11 CHOSEN_API == 2
#define API_D3D12 CHOSEN_API == 3

#if API_GL
#include "OpenGL/OpenGLRHI.h"
#elif API_D3D11
#include "D3D11/D3D11RHI.h"
#elif API_D3D12
#include "D3D12/D3D12RHI.h"
#endif

class NullRHI : public RHI
{};

void RHI::Init()
{
#if API_GL
	Instance = new OpenGLRHI();
#elif API_D3D11
	Instance = new D3D11RHI();
#elif API_D3D12
	Instance = new D3D12RHI();
#endif
}

void RHI::Terminate()
{
	delete Get();
}
