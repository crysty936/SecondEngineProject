#pragma once
#include "Renderer/RHI/Resources/RHITexture.h"

class D3D11Texture2D : public RHITexture2D
{
public:
	D3D11Texture2D(struct ID3D11Texture2D* inHandle);

public:
	struct ID3D11Texture2D* Handle = nullptr;
	struct ID3D11ShaderResourceView* ResourceViewHandle = nullptr;
};