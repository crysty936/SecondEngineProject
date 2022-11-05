#include "D3D11Shader.h"

D3D11Shader::D3D11Shader(ID3D11VertexShader* inVertexHandle, ID3D11PixelShader* inPixelHandle, ID3D11InputLayout* inVertexInputLayout)
	: VertexShaderHandle(inVertexHandle), PixelShaderHandle(inPixelHandle), VertexInputLayout(inVertexInputLayout) {}

