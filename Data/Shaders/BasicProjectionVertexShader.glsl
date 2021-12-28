#version 330 core 

layout(location = 0) in vec3 aPosition;
//layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTexCoords;

//out vec3 INormal;
//out vec3 IFragPos;
//out vec2 ITexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//ITexCoords = aTexCoords;
	//INormal = mat3(transpose(inverse(model))) * aNormal;
	//IFragPos = vec3(model * vec4(aPosition, 1.0));

	gl_Position= projection * view * /*model **/ vec4(aPosition , 1.0);

}