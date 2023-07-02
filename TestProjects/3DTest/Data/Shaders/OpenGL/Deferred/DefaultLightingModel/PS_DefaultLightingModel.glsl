#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoords;
} ps_in;


layout(std140, binding = 1) uniform ViewConstantsBuffer
{
	mat4 perspInv;
	mat4 viewInv;
	vec4 CameraNearFar;
	vec4 ViewportSize;

} ViewConstants;

layout(binding = 0) uniform sampler2D GBufferAlbedo;
layout(binding = 1) uniform sampler2D GBufferNormal;
layout(binding = 2) uniform sampler2D GBufferDepth;

void main()
{
	float depthValue = texture(GBufferDepth, ps_in.TexCoords).r;

	//vec3 color = texture(GBufferAlbedo, ps_in.TexCoords).rgb;
	vec3 normal = texture(GBufferNormal, ps_in.TexCoords).rgb;

	vec2 clipCoord = ps_in.TexCoords.xy * 2.f - 1.f;
	vec4 clipsSpaceLoc = vec4(clipCoord.x, clipCoord.y, depthValue, 1.0);

	vec4 viewSpacePos = ViewConstants.perspInv * clipsSpaceLoc;
	viewSpacePos /= viewSpacePos.w;
	vec4 worldSpacePos = ViewConstants.viewInv * viewSpacePos;

	//float depthValue = texture(quadTexture, inTexCoords).r;


	FragColor = vec4(worldSpacePos.xyz, 1.0);
}