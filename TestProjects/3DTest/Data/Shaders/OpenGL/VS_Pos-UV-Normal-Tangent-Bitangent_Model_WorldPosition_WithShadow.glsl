#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoords;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

out VS_OUT
{
	vec2 TexCoords;
	mat4 clipToWorldMatrix;
	vec3 worldPos;
	vec3 viewPos;
	vec3 Normal;
	mat3 TangentToWorld;
	flat vec3 DirectionalLightDirection;
	mat4 ShadowCameraViewMatrix;
	mat4 lsMatrices[3];
	flat int cascadesCount;
	flat int bShadowVisualizeMode;
	flat int bNormalVisualizeMode;
	flat int bUseNormalMapping;
	flat int bUseShadows;
	flat float shadowCascadeFarPlanes[3];
} vs_out;

layout(std140, binding = 0) uniform GeometryDataBuffer
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

layout(std140, binding = 1) uniform ShadowDataBuffer
{
	mat4 lsMatrices[3];
	mat4 ShadowCameraViewMatrix;
	vec4 DirectionalLightDirection;
	int cascadesCount;
	int bShadowVisualizeMode;
	int bNormalVisualizeMode;
	int bUseNormalMapping;
	int bUseShadows;
	float shadowCascadeFarPlanes[3];
};

layout(std140, binding = 2) uniform LightingDataBuffer
{
	vec3 viewPos;
};

void main()
{
	vec3 fragPos = vec3(model * vec4(inPosition, 1.0));
	vs_out.lsMatrices = lsMatrices;
	vs_out.worldPos = fragPos;
	vs_out.TexCoords = inTexCoords;
	vs_out.clipToWorldMatrix = inverse(projection * view);
	vs_out.Normal = mat3(transpose(inverse(model))) * inNormal;
	vs_out.DirectionalLightDirection = DirectionalLightDirection.xyz;
	vs_out.ShadowCameraViewMatrix = ShadowCameraViewMatrix;
	vs_out.cascadesCount = cascadesCount;
	vs_out.bShadowVisualizeMode = bShadowVisualizeMode;
	vs_out.bNormalVisualizeMode = bNormalVisualizeMode;
	vs_out.bUseNormalMapping = bUseNormalMapping;
	vs_out.shadowCascadeFarPlanes = shadowCascadeFarPlanes;
	vs_out.bUseShadows = bUseShadows;
	vs_out.viewPos = viewPos;

	// Gram - Schmidt process
	// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	vec3 T = normalize(vec3(model * vec4(inTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(inNormal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);
	vs_out.TangentToWorld = mat3(T, B, N);

	gl_Position = projection * view * vec4(fragPos, 1.0);
}