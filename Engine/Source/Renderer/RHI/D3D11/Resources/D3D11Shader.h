#pragma once
#include "Renderer/RHI/Resources/RHIShader.h"

class D3D11Shader : public RHIShader
{
public:
	D3D11Shader(struct ID3D11VertexShader* inVertexHandle, struct ID3D11PixelShader* inPixelHandle, struct ID3D11InputLayout* inVertexInputLayout);

public:
	struct ID3D11VertexShader* VertexShaderHandle = nullptr;
	struct ID3D11PixelShader* PixelShaderHandle = nullptr;
	struct ID3D11GeometryShader* GeometryShaderHandle = nullptr;
	struct ID3D11InputLayout* VertexInputLayout = nullptr;
};