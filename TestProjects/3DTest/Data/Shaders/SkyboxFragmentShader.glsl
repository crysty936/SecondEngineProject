#version 330 core 
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) in vec3 inTexCoords;
layout(location = 0) out vec4 FragColor;

uniform samplerCube cubeMap;

void main()
{
	vec4 color = texture(cubeMap, inTexCoords);

	FragColor = color;
}