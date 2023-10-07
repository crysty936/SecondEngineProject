#include "Math/BVH.h"

BVHNode::BVHNode() = default;

BVHNode::~BVHNode()
{
	delete LeftNode, RightNode;
}

void BVHNode::DebugDraw() const
{
	BoundingBox.DebugDraw();
}

BVH::BVH() = default;

BVH::~BVH()
{
	delete Root;
}

#define TERMINATION_SIZE 2

void RecursivelyBuildBVH(BVHNode& inNode, const eastl::vector<PathTraceTriangle>& inTriangles)
{
	if (inTriangles.size() == 1)
	{
		inNode.BoundingBox = inTriangles[0].CalculateBoundingBox();
		inNode.Triangles = inTriangles;

		return;
	}

	if (inTriangles.size() <= TERMINATION_SIZE)
	{
		inNode.Triangles = inTriangles;
		for (const PathTraceTriangle& triangle : inTriangles)
		{
			inNode.BoundingBox += triangle.CalculateBoundingBox();
		}

		return;
	}



	glm::vec3 center(0.f, 0.f, 0.f);

	eastl::vector<glm::vec3> triangleCenters;
	triangleCenters.reserve(inTriangles.size());

	const float invTriangleCount = 1.f / inTriangles.size();



	




}


void BVH::Build(const eastl::vector<PathTraceTriangle>& inTriangles)
{
	LOG_INFO("Building BVH");

	Root = new BVHNode();

	RecursivelyBuildBVH(*Root, inTriangles);

}

