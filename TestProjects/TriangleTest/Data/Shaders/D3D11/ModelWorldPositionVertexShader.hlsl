
cbuffer ConstantBuffer : register(b0)
{
	matrix Projection;
	matrix View;
	matrix Model;
}

struct VS_INPUT
{
	float4 Pos : POSITION;
	//float3 Norm : NORMAL;
	//float2 Coord : TEXCOORD;
};


float4 VS(VS_INPUT input) : SV_POSITION
{
	//float4 output = mul(input.Pos, Model);
	//output = mul(output, View);
	//output = mul(output, Projection);

	float4 output = mul(Model, input.Pos);
	output = mul(View, output);
	output = mul(Projection, output);


    return output;
}
