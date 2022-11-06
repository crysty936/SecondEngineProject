#version 420 core 
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
//layout(location = 1) in vec3 aNormal;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec2 outTexCoords;
layout(location = 1) out vec4 outFragPos;

layout(std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

void main()
{
	outTexCoords = inTexCoords;
	//INormal = mat3(transpose(inverse(model))) * aNormal;
	//IFragPos = vec3(model * vec4(aPosition, 1.0));

	//vec4 canonicalViewPos = vec4(aPosition, 1.0);
	vec4 canonicalViewPos = projection * view * model * vec4(aPosition, 1.0);

	//canonicalViewPos.z = 1.0;

	outFragPos = canonicalViewPos;
	gl_Position = canonicalViewPos;
}