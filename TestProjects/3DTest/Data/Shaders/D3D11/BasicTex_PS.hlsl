
Texture2D TexDiffuse;
SamplerState TexDiffuse_sampler;

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};


float4 SampleDiffuse(PS_INPUT input)
{
	float2 UVs = input.TexCoord;
	float4 color = TexDiffuse.Sample(TexDiffuse_sampler, UVs);

	return color;
}

float4 PSEntry(PS_INPUT input) : SV_Target
{
	float4 color = SampleDiffuse(input);
	
	return color;
}
