#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoords;
} ps_in;

uniform sampler2D quadTexture;

void main()
{
	vec3 hdrColor = texture(quadTexture, ps_in.TexCoords).xyz;
	float brightness = dot(hdrColor, vec3(0.2126, 0.7152, 0.0722));
	vec3 color;
	if (brightness > 1.0)
		color = hdrColor.rgb;
	else
		color = vec3(0.0, 0.0, 0.0);

	FragColor = vec4(color, 1.0);
}