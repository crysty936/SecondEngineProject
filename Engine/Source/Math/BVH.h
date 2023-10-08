#pragma once
#include "glm/common.hpp"
#include "glm/ext/vector_float3.hpp"
#include "Core/EngineUtils.h"
#include "EASTL/array.h"
#include "AABB.h"
#include "Math/PathTracing.h"

struct BVHNode
{
	BVHNode();
	~BVHNode();

	AABB BoundingBox;

	struct BVHNode* LeftNode = nullptr;
	struct BVHNode* RightNode = nullptr;

	eastl::vector<PathTraceTriangle> Triangles;

	bool Intersects(const PathTracingRay& inRay);


	void DebugDraw() const;

};

struct BVH
{
	BVH();
	~BVH();

	void Build(const eastl::vector<PathTraceTriangle>& inTriangles);

	bool Intersects(const PathTracingRay& inRay);

	inline bool IsValid() { return Root != nullptr; }

	BVHNode* Root = nullptr;
};