#version 330 core 
layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor= vec4(vec3(gl_FragCoord.z), 1.0);
}