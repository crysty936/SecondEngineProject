#version 330 core 
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec2 inTexCoords;

uniform sampler2D quadTexture;

void main()
{
	vec4 color = texture(quadTexture, inTexCoords);

	FragColor = color;
}