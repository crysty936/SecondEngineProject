#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 gNormal;
layout(location = 1) out vec4 gAlbedo;
layout(location = 2) out vec4 gMetallicRoughness;

in VS_OUT
{
	vec2 TexCoords;
	mat4 clipToWorldMatrix;
	vec3 worldPos;
	vec3 VertexNormal;
	mat3 TangentToWorld;
} ps_in;

layout(std140, binding = 1) uniform CascadedShadowDataBuffer
{
	float Metallic;
	float Roughness;
} MatProperties;



void main()
{

	gNormal = vec4(ps_in.VertexNormal / 2.0 + 0.5, 1.0);;
	gAlbedo = vec4(1.0, 0.0, 0.0, 1.0);
	gMetallicRoughness = vec4(MatProperties.Metallic, MatProperties.Roughness, 0.0, 0.0);
}