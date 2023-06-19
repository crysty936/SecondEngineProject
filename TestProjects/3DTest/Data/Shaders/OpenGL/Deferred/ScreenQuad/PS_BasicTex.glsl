#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoords;
} ps_in;

uniform sampler2D quadTexture;

void main()
{
	float depthValue = texture(quadTexture, inTexCoords).r;
	FragColor = vec4(vec3(depthValue), 1.0);
}