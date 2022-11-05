
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Coord : TEXCOORD;
};


float4 VS(VS_INPUT input) : SV_POSITION
{
	float4 output = mul(input.Pos, World);
	output = mul(output, View);
	output = mul(output, Projection);

    return output;
}
