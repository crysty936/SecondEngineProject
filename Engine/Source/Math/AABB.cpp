#include "Math/AABB.h"

eastl::array<glm::vec3, 8> AABB::GetVertices()
{
	// Use a unit cube
	const glm::vec3 verts[8] = {
		// left side
		glm::vec3(1, 1, 1),// 0 left top forward
		glm::vec3(1, 1,-1),// 1 left bottom forward
		glm::vec3(1,-1, 1),// 2 left top back
		glm::vec3(1,-1,-1),// 3 left bottom back

		// right side
		glm::vec3(-1, 1, 1),// 4 right top forward
		glm::vec3(-1, 1,-1),// 5 right bottom forward
		glm::vec3(-1,-1, 1),// 6 right top back
		glm::vec3(-1,-1,-1) // 7 right bottom back
	};

	eastl::array<glm::vec3, 8> worldPoints;
	glm::vec3 boxCenter, boxExtent;
	GetCenterAndExtent(boxCenter, boxExtent);

	// Get the bb's world vertices
	for (int32_t i = 0; i < 8; i++)
	{
		const glm::vec3 worldVert = boxCenter + (verts[i] * boxExtent);
		worldPoints[i] = worldVert;
	}

	return worldPoints;
}
