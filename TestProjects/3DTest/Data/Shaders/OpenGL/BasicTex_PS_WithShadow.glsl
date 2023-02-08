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

vec2 poissonDisk[4] = vec2[](
	vec2(-0.94201624, -0.39906216),
	vec2(0.94558609, -0.76890725),
	vec2(-0.094184101, -0.92938870),
	vec2(0.34495938, 0.29387760)
	);


void main()
{
	//float depthValue = texture(quadTexture, inTexCoords).r;
	//FragColor = vec4(vec3(depthValue), 1.0);

	vec4 worldPos = vec4(ps_in.worldPos, 1.0);

	vec4 lsPos = ps_in.lsMatrix * worldPos;

	vec3 lsPosFinal = lsPos.xyz / lsPos.w;
	// remap from -1..1 to 0..1
	vec3 projCoords = lsPosFinal * 0.5 + 0.5;

	//float shadowSamplerTest = texture(depthTexture, vec3(projCoords.xy, projCoords.z + 0.001));


	float pixelLightSpaceDepth = lsPosFinal.z;
	// 2 different ways of calculating bias 
	//float cosTheta = clamp(dot(ps_in.Normal, ps_in.DirectionalLightDirection), 0.0, 1.0);
	//float bias2 = 0.005 * tan(acos(cosTheta));
	//bias2 = clamp(bias2, 0.0, 0.01);

	float bias1 = max(0.005 * (1.0 - clamp(dot(ps_in.Normal, ps_in.DirectionalLightDirection), 0.0, 1.0)), 0.005);
	vec2 texelSize = 1.0 / textureSize(depthTexture, 0);

	//float shadow = 0.0;
	//float textureDepth = texture(depthTexture, projCoords.xy).r;
	//if ((pixelLightSpaceDepth - bias1 > textureDepth))
	//{
	//	shadow = 1.0;
	//}

	//float shadow = 0.0;
	//for (int x = -1; x <= 1; ++x)
	//{
	//	for (int y = -1; y <= 1; ++y)
	//	{
	//		float textureDepth = texture(depthTexture, projCoords.xy + vec2(x, y) * texelSize).r;
	//		shadow += pixelLightSpaceDepth - bias1 > textureDepth ? 1.0 : 0.0;
	//	}
	//}
	//shadow /= 9.0;

	//float shadow = 0.0;
	//float textureDepth = texture(depthTexture, projCoords.xy).r;
	//if (pixelLightSpaceDepth < 1.0)
	//{
	//	if ((pixelLightSpaceDepth - bias1 > textureDepth))
	//	{
	//		shadow = 1.0;
	//	}
	//}

	//float shadow = 0.0;
	//if (pixelLightSpaceDepth < 1.0)
	//{
	//	for (int i = 0; i < 4; i++) 
	//	{
	//		float textureDepth = texture(depthTexture, projCoords.xy + poissonDisk[i] / 500.0).r;

	//		if (pixelLightSpaceDepth - bias1 > textureDepth) {
	//			shadow += 0.2;
	//		}
	//	}
	//}
	//

	
	// Vogel disk PCF
	float shadow = 0.0;
	if (pixelLightSpaceDepth < 1.0)
	{
		const int numSamples = 32;
		float diskRadius = 4;

		for (int i = 0; i < numSamples; i++)
		{
			float GoldenAngle = 2.39996322;
			float angle = GoldenAngle * float(i);

			float r = sqrt(float(i) + 0.5) / sqrt(float(numSamples));

			vec2 u = r * vec2(sin(angle), cos(angle));
			vec2 pos = u * diskRadius * texelSize;

			float textureDepth = texture(depthTexture, projCoords.xy + pos).r;

			if (pixelLightSpaceDepth - bias1 > textureDepth)
			{
				shadow += 1;
			}
		}

		shadow /= numSamples;
	}
	


	vec3 color = (0.8 * ((1 - shadow) * texture(quadTexture, ps_in.TexCoords).xyz)) + (0.2 * texture(quadTexture, ps_in.TexCoords).xyz);
	//vec3 color = vec3(1 - shadow, 1 - shadow, 1 - shadow);
	FragColor = vec4(color, 1.0);
}