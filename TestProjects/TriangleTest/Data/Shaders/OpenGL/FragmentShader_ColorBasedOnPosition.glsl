#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 FragColor;

layout(location = 1) in vec4 inFragPos;

void main()
{
	FragColor = vec4(inFragPos.xyz, 1.0);
}