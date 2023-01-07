#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoords;

layout(location = 0) out vec2 outTexCoords;

void main()
{
	outTexCoords = inTexCoords;
	gl_Position = vec4(inPosition, 1.0);
}