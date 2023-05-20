#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoords;
} ps_in;

layout(std140, binding = 1) uniform ViewConstantsBuffer
{
	vec2 CameraNearFar;

} ViewConstants;


uniform sampler2D quadTexture;

void main()
{
	float depthValue = texture(quadTexture, ps_in.TexCoords).r;

	//- (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
	//(2.0 * near * far) / (far + near - z * (far - near));

	//-(zFar * zNear) / (zFar - zNear);



	float near = ViewConstants.CameraNearFar.x;
	float far = ViewConstants.CameraNearFar.y;
	float normalizedDepth = (2.0 * near * far) / (far + near - depthValue * (far - near));;


	FragColor = vec4(vec3(normalizedDepth), 1.0);
}