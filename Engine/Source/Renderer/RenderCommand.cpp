#include "RenderCommand.h"

void Triangle::Transform(const glm::mat4& inMatrix)
{
	for (int32_t i = 0; i < 3; ++i)
	{
		glm::vec3& currentVert = V[i];

		const glm::vec4 fourRowVec = glm::vec4(currentVert.x, currentVert.y, currentVert.z, 1.f);
		const glm::vec4 res = inMatrix * fourRowVec;

		currentVert = glm::vec3(res.x, res.y, res.z);
	}

	E[0] = V[1] - V[0];
	E[1] = V[2] - V[0];

	WSNormal = glm::cross(E[0], E[1]);
}
