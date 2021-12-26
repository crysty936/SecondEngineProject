#version 330 core 

layout(location = 0) in vec3 aPosition;

out vec3 INormal;
out vec3 IFragPos;
out vec2 ITexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

	gl_Position = vec4(aPosition , 1.0);

}