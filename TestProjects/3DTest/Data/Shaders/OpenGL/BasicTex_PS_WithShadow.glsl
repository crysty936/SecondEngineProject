#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoords;
	mat4 lsMatrix;
	mat4 clipToWorldMatrix;
	vec3 worldPos;
	vec3 Normal;
	vec3 DirectionalLightDirection;
} ps_in;

layout(binding = 0) uniform sampler2D quadTexture;
layout(binding = 1) uniform sampler2D depthTexture;

void main()
{
	//float depthValue = texture(quadTexture, inTexCoords).r;
	//FragColor = vec4(vec3(depthValue), 1.0);

	vec4 worldPos = vec4(ps_in.worldPos, 1.0);

	vec4 lsPos = ps_in.lsMatrix * worldPos;

	vec3 lsPosFinal = lsPos.xyz / lsPos.w;
	vec3 projCoords = lsPosFinal * 0.5 + 0.5;

	//float shadowSamplerTest = texture(depthTexture, vec3(projCoords.xy, projCoords.z + 0.001));
	float closestDepth = texture(depthTexture, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float bias = max(0.01 * (1.0 - dot(ps_in.Normal, ps_in.DirectionalLightDirection)), 0.005);

	float shadow = 0.0;
	if ((closestDepth < currentDepth ) && currentDepth < 1.0)
	{
		shadow = 1.0;
	}
	else
	{
		shadow = 0.0;
	}


	//FragColor = vec4(ps_in.worldPos, 1.0);

	//FragColor = vec4(lsPosFinal, 1.0);
	//FragColor = vec4(lsPosFinal, 1.0);
	//FragColor = vec4(projCoords, 1.0);
	//FragColor = vec4(closestDepth, closestDepth, closestDepth, 1.0);
	//FragColor = vec4(currentDepth, currentDepth, currentDepth, 1.0);
	//FragColor = vec4(test, test, test, 1.0);
	//FragColor = vec4(shadow, shadow, shadow, 1.0);

	//vec3 color = (test * texture(quadTexture, ps_in.TexCoords)).xyz;
	vec3 color = ((1 - shadow) * texture(quadTexture, ps_in.TexCoords)).xyz;
	FragColor = vec4(color, 1.0);
}