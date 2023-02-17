layout(triangles, invocations = 3) in;
layout(triangle_strip, max_vertices = 3) out;

layout(std140, binding = 1) uniform ConstantBuffer
{
	mat4 worldToLightClipMatrices[3];
};

void main()
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position =
			worldToLightClipMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}
	EndPrimitive();
}