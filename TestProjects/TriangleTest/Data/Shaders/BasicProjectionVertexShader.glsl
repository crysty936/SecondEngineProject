#version 330 core 
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec4 outVertPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	outTexCoords = inTexCoords;
	vec4 vertPos = projection * view * model * vec4(aPosition, 1.0);
	outVertPos = vertPos;

	gl_Position = vertPos;
}