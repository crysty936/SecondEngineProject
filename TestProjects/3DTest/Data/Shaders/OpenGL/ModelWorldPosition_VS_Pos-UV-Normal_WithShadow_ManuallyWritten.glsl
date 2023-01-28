#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

out VS_OUT
{
	vec2 TexCoords;
	mat4 lsMatrix;
	mat4 clipToWorldMatrix;
	vec3 worldPos;
	vec3 Normal;
	vec3 DirectionalLightDirection;
} vs_out;

layout(std140, binding = 0) uniform GeometryDataBuffer
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

layout(std140, binding = 1) uniform LightDataBuffer
{
	mat4 lsMatrix;
	vec3 DirectionalLightDirection;
};

void main()
{
	vec3 fragPos = vec3(model * vec4(inPosition, 1.0));
	vs_out.worldPos = fragPos;
	vs_out.TexCoords = inTexCoords;
	vs_out.lsMatrix = lsMatrix;
	vs_out.clipToWorldMatrix = inverse(projection * view);
	vs_out.Normal = mat3(transpose(inverse(model))) * inNormal;
	vs_out.DirectionalLightDirection = DirectionalLightDirection;

	gl_Position = projection * view * vec4(fragPos, 1.0);
}