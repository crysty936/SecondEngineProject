#version 330 core 
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec2 TexCoords;
layout(location = 1) in vec3 Normal;

uniform sampler2D tex1;

void main()
{
	FragColor = texture(tex1, TexCoords);
}