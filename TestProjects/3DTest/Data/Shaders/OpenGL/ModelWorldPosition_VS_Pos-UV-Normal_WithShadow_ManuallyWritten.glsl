#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

out VS_OUT
{
	vec2 TexCoords;
	mat4 clipToWorldMatrix;
	vec3 worldPos;
	vec3 Normal;
	vec3 DirectionalLightDirection;
	mat4 ShadowViewMatrix;
	mat4 lsMatrices[3];
	float bVisualizeMode;
} vs_out;

layout(std140, binding = 0) uniform GeometryDataBuffer
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

layout(std140, binding = 1) uniform LightDataBuffer
{
	mat4 lsMatrices[3];
	vec3 DirectionalLightDirection;
	mat4 ShadowViewMatrix;
	float bVisualizeMode;
};

void main()
{
	vec3 fragPos = vec3(model * vec4(inPosition, 1.0));
	vs_out.lsMatrices = lsMatrices;
	vs_out.worldPos = fragPos;
	vs_out.TexCoords = inTexCoords;
	vs_out.clipToWorldMatrix = inverse(projection * view);
	vs_out.Normal = mat3(transpose(inverse(model))) * inNormal;
	vs_out.DirectionalLightDirection = DirectionalLightDirection;
	vs_out.ShadowViewMatrix = ShadowViewMatrix;
	vs_out.bVisualizeMode = bVisualizeMode;

	gl_Position = projection * view * vec4(fragPos, 1.0);
}