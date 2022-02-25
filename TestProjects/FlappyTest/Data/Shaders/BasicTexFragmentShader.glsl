#version 330 core 
layout(location = 0) out vec4 FragColor;
in vec2 outTexCoords;

uniform sampler2D tex1;

void main()
{
	FragColor = texture(tex1, outTexCoords);
}