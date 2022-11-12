#include "D3D11Texture2D.h"
#include <d3d11.h>

D3D11Texture2D::D3D11Texture2D(ID3D11Texture2D* inHandle)
	: Handle(inHandle), RHITexture2D()
{
}


