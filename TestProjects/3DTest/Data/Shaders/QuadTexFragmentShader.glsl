#version 330 core 
out vec4 FragColor;
in vec2 outTexCoords;

uniform sampler2D quadTexture;

void main()
{
	FragColor = vec4(vec3(1.0 - texture(quadTexture, outTexCoords)), 1.0);
}