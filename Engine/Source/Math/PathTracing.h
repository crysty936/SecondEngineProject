#pragma once
#include "glm/common.hpp"
#include "glm/ext/vector_float3.hpp"
#include "Core/EngineUtils.h"
#include "EASTL/array.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "AABB.h"

struct PathTraceTriangle
{
	//Triangle(const glm::vec3& inV0, const glm::vec3& inV1, const glm::vec3& inV2)
	//	: V0(inV0), V1(inV1), V2(inV2)
	//{
	//	E0 = V1 - V0;
	//	E1 = V2 - V0;
	//	WSNormal = glm::normalize(glm::cross(E0, E1));
	//}

	PathTraceTriangle(glm::vec3 inVerts[3])
	{
		V[0] = inVerts[0];
		V[1] = inVerts[1];
		V[2] = inVerts[2];

		E[0] = V[1] - V[0];
		E[1] = V[2] - V[0];

		WSNormal = glm::cross(E[0], E[1]);
	}

	glm::vec3 V[3];
	glm::vec3 E[2];

	//glm::vec3 V0, V1, V2;
	//glm::vec3 E0, E1;
	glm::vec3 WSNormal;


	void Transform(const glm::mat4& inMatrix);

	AABB CalculateBoundingBox() const;

	//inline Triangle& operator*(const glm::mat4& inMatrix)
	//{


	//	return *this;
	//}

	//glm::vec2 UVs;


};
