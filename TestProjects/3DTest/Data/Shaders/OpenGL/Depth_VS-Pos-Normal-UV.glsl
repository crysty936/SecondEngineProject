#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

layout(std140, binding = 0) uniform ConstantBuffer
{
	mat4 lsMatrix;
	mat4 model;
};

void main()
{
	vec3 fragPos = vec3(model * vec4(inPosition, 1.0));
	gl_Position = lsMatrix * vec4(fragPos, 1.0);
}