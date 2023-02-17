#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "EASTL/vector.h"
#include "Utils/InlineVector.h"
#include "EASTL/array.h"

struct DebugLine
{
	glm::vec3 Start;
	glm::vec3 End;
	glm::vec3 Color;
};

class DrawDebugManager
{
public:
	static DrawDebugManager& Get()
	{
		static DrawDebugManager Instance;

		return Instance;
	}

	void AddDebugPoint(const glm::vec3& inPoint);
	void AddDebugLine(const DebugLine& inLine);

private:
	const eastl::vector<glm::vec3>& GetPoints() const { return DebugPoints; }
	const eastl::vector<DebugLine>& GetLines() const { return DebugLines; }
	
	void ClearDebugData() 
	{ 
		DebugPoints.clear();
		DebugLines.clear();
	}

	friend class ForwardRenderer;
private:
	eastl::vector<glm::vec3> DebugPoints;
	eastl::vector<DebugLine> DebugLines;
};

namespace DrawDebugHelpers
{
	void DrawDebugPoint(const glm::vec3 inPoint);
	void DrawDebugLine(const DebugLine& inLine);
	void DrawProjectionPoints(const glm::mat4& inProj);
	void DrawLinesArray(const eastl::vector<glm::vec3>& inLinesPoints, const glm::vec3& inColor = glm::vec3(0.5f, 0.5f, 0.f));
	void DrawBoxArray(eastl::array<glm::vec3, 8> inArray, const glm::vec3& inColor = glm::vec3(0.5f, 0.5f, 0.f));
	void DrawProjection(const glm::mat4& inProj);
}