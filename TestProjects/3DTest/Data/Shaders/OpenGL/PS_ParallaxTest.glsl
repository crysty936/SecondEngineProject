#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoords;
	mat4 clipToWorldMatrix;
	vec3 FragWorldPos;
	vec3 FragTangentPos;
	vec3 Normal;
	mat3 WorldToTangent;
	flat vec3 DirectionalLightDirectionTS;
	flat int bNormalVisualizeMode;
	flat int bUseNormalMapping;
	flat float ParallaxHeightScale;
	vec3 TSViewPos;
} ps_in;

layout(binding = 0) uniform sampler2D DiffuseMap;
layout(binding = 1) uniform sampler2D NormalMap;
layout(binding = 2) uniform sampler2D DispMap;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	float height = texture(DispMap, ps_in.TexCoords).r;

	// Multiply p based on z and height, control with ParallaxHeightScale
	vec2 p = viewDir.xy / viewDir.z * (height * ps_in.ParallaxHeightScale);

	// Everything is in tangent space so x and y of p are as UV's
	return texCoords - p;
}

void main()
{
	vec3 color;
	if (bool(ps_in.bNormalVisualizeMode))
	{
		vec3 mapNormal = texture(NormalMap, ps_in.TexCoords).xyz;
		// to direction vector
		mapNormal = mapNormal * 2.0 - 1.0;
		vec3 wsNormal = normalize(transpose(ps_in.WorldToTangent) * mapNormal);
		// back to color
		wsNormal = wsNormal * 0.5 + 0.5;

		color = wsNormal;
	}
	else
 	{
		vec2 parallaxUVs = ParallaxMapping(ps_in.TexCoords, ps_in.FragWorldPos - ps_in.TSViewPos);


		if (parallaxUVs.x > 1.0 || parallaxUVs.y > 1.0 || parallaxUVs.x < 0.0 || parallaxUVs.y < 0.0)
			discard;

		vec3 diffColor = texture(DiffuseMap, parallaxUVs).xyz;
		if (bool(ps_in.bUseNormalMapping))
		{
			vec3 mapNormal = texture(NormalMap, parallaxUVs).xyz;
			mapNormal = mapNormal * 2.0 - 1.0;

			float diffPower = max(dot(mapNormal, ps_in.DirectionalLightDirectionTS), 0.3);
			vec3 dirLightColor = diffPower * diffColor;
 			color = dirLightColor + 0.2 * diffColor;
		}
		else
		{
			color = diffColor;
		}
		
 	}

	//vec3 color = texture(DispMap, ps_in.TexCoords).xyz;

	FragColor = vec4(color, 1.0);
}