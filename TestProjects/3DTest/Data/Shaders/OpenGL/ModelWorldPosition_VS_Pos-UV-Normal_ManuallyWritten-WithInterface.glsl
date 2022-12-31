#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;

out VS_OUT{
	vec3 normal;
} vs_out;

layout(std140, binding = 0) uniform ConstantBuffer
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

void main()
{
	//outTexCoords = inTexCoords;
	vec3 fragPos = vec3(model * vec4(inPosition, 1.0));

	mat3 normalMatrix = mat3(transpose(inverse( model)));
	vs_out.normal = normalize(vec3(vec4(normalMatrix * inNormal, 0.0)));

	gl_Position = vec4(fragPos, 1.0);
}