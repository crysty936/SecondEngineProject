
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	float4 output = mul(Pos, World);
	output = mul(output, View);
	output = mul(output, Projection);

    return output;
}
