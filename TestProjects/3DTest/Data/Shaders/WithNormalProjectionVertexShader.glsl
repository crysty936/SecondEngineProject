#version 330 core 
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 inTexCoords;

//out vec3 IFragPos;
layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	outTexCoords = inTexCoords;
	Normal = mat3(transpose(inverse(model))) * aNormal;
	//IFragPos = vec3(model * vec4(aPosition, 1.0));

	gl_Position= projection * view * model * vec4(aPosition , 1.0);

}