
// 16 byte aligned
cbuffer RootConstantBuffer : register(b0)
{
    float4 theTest;
}

// 256 byte aligned
cbuffer SceneConstantBuffer : register(b1)
{
    float4x4 Model;
    float4x4 Projection;
    float padding[32];
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

PSInput VSMain(float4 position : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    PSInput result;

    result.uv = uv;
    // Inverse y to simulate opengl like uv space
    result.uv.y = 1 - uv.y;

    result.position = mul(Model, position);

    result.position = mul(Projection, position);
    
    //result.position.x += offset;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return g_texture.Sample(g_sampler, input.uv);
}
