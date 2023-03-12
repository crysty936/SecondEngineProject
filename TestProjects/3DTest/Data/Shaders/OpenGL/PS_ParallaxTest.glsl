#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoords;
	mat4 clipToWorldMatrix;
	vec3 worldPos;
	vec3 Normal;
	mat3 WorldToTangent;
	flat vec3 DirectionalLightDirection;
	flat int bNormalVisualizeMode;
} ps_in;

layout(binding = 0) uniform sampler2D DiffuseMap;
layout(binding = 1) uniform sampler2D NormalMap;
layout(binding = 2) uniform sampler2D DispMap;

void main()
{
	//vec4 worldPos = vec4(ps_in.worldPos, 1.0);
	//vec3 color;
	//if (bool(ps_in.bNormalVisualizeMode))
	//{
	//	vec3 mapNormal = texture(NormalMap, ps_in.TexCoords).xyz;
	//	mapNormal = mapNormal * 2.0 - 1.0;
	//	vec3 wsNormal = normalize(ps_in.TangentToWorld * mapNormal);
	//	color = wsNormal;
	//}
	//else
 //	{
	//	mat3 WorldToTangent = transpose(ps_in.TangentToWorld);

	//	vec3 ambientColor = texture(DiffuseMap, ps_in.TexCoords).xyz;

	//	vec3 mapNormal = texture(NormalMap, ps_in.TexCoords).xyz;
	//	mapNormal = mapNormal * 2.0 - 1.0;
	//	//vec3 wsNormal = normalize(ps_in.TangentToWorld * mapNormal);
	//	vec3 tsLightDir = normalize(WorldToTangent * ps_in.DirectionalLightDirection);
	//	float diffPower = max(dot(mapNormal, tsLightDir), 0.3);

	//	vec3 dirLightColor = diffPower * texture(DiffuseMap, ps_in.TexCoords).xyz;

 //		color = dirLightColor + 0.2 * ambientColor;
 //	}

	vec3 color = texture(DispMap, ps_in.TexCoords).xyz;

	FragColor = vec4(color, 1.0);
}