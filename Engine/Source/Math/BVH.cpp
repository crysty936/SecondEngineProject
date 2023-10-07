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

void RecursivelyBuildBVH(BVHNode& inNode, const eastl::vector<PathTraceTriangle>& inTriangles)
{
	if (inTriangles.size() == 1)
	{
		inNode.BoundingBox = inTriangles[0].CalculateBoundingBox();
		inNode.Triangles = inTriangles;

		return;
	}







}


void BVH::Build(const eastl::vector<PathTraceTriangle>& inTriangles)
{
	LOG_INFO("Building BVH");

	Root = new BVHNode();

	RecursivelyBuildBVH(*Root, inTriangles);

}

