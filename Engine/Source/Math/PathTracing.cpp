#include "Math/PathTracing.h"

void PathTraceTriangle::Transform(const glm::mat4& inMatrix)
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

	WSNormal = glm::normalize(glm::cross(E[0], E[1]));
}

AABB PathTraceTriangle::GetBoundingBox() const
{
	AABB res;

	for (int32_t i = 0; i < 3; ++i)
	{
		res += V[i];
	}

	return res;
}

bool TraceTriangle(const PathTracingRay& inRay, const PathTraceTriangle& inTri)
{
	const glm::vec3& A = inTri.V[0];
	const glm::vec3& E1 = inTri.E[0];
	const glm::vec3& E2 = inTri.E[1];
	const glm::vec3& N = inTri.WSNormal;

	float det = -dot(inRay.Direction, N);
	float invdet = 1.f / det;

	glm::vec3 AO = inRay.Origin - A;
	glm::vec3 DAO = glm::cross(AO, inRay.Direction);

	float u = dot(E2, DAO) * invdet;
	float v = -dot(E1, DAO) * invdet;
	float t = dot(AO, N) * invdet;
	return (det >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0);

	//const Payload result = { closestSphere, worldPosition, closestDistance };

	//return result;
}
