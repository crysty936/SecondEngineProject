#pragma once
#include "Renderer/DrawType.h"
#include "EASTL/shared_ptr.h"

#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "EASTL/vector.h"
#include "glm/ext/matrix_float4x4.hpp"

struct Triangle
{
	//Triangle(const glm::vec3& inV0, const glm::vec3& inV1, const glm::vec3& inV2)
	//	: V0(inV0), V1(inV1), V2(inV2)
	//{
	//	E0 = V1 - V0;
	//	E1 = V2 - V0;
	//	WSNormal = glm::normalize(glm::cross(E0, E1));
	//}

	Triangle(glm::vec3 inVerts[3])
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

	//inline Triangle& operator*(const glm::mat4& inMatrix)
	//{


	//	return *this;
	//}

	//glm::vec2 UVs;


};

namespace EDrawMode
{
	enum Type : uint8_t
	{
		Default = 1 << 0,
		DEPTH = 1 << 2,
		DEPTH_VISUALIZE = 1 << 3,
		OUTLINE = 1 << 4,
		NORMAL_VISUALIZE = 1 << 5,
		Count = 1 << 6
	};
}

struct RenderCommand
{
	eastl::shared_ptr<class MeshDataContainer> DataContainer;
	eastl::shared_ptr<class RenderMaterial> Material;
	eastl::weak_ptr<const class DrawableObject> Parent;

	EDrawType DrawType = EDrawType::DrawElements;
	EDrawMode::Type DrawPasses = EDrawMode::Default;

	/** In case draw mode is DrawInstanced */
	int32_t InstancesCount = 0;


	// FOR PATHTRACER, TO MOVE

	eastl::vector<Triangle> Triangles;
};

