#include "DrawDebugHelpers.h"
#include "Utils/InlineVector.h"
#include "ForwardRenderer.h"
#include "RenderUtils.h"

void DrawDebugHelpers::DrawDebugPoint(const glm::vec3 inPoint)
{
	DrawDebugManager::Get().AddDebugPoint(inPoint);
}

void DrawDebugHelpers::DrawDebugLine(const DebugLine& inLine)
{
	DrawDebugManager::Get().AddDebugLine(inLine);
}

void DrawDebugHelpers::DrawProjectionPoints(const glm::mat4& inProj)
{
	eastl::array<glm::vec3, 8> projPoints = RenderUtils::GenerateSpaceCorners(inProj, 0.f, 1.f);

	for (int i = 0; i < 8; ++i)
	{
		const glm::vec3& currentPoint = projPoints[i];
		DrawDebugPoint(currentPoint);
	}
}

void DrawDebugHelpers::DrawLinesArray(const eastl::vector<glm::vec3>& inLinesPoints, const glm::vec3& inColor)
{
	for (int32_t vertexIndex = 0; vertexIndex < inLinesPoints.size(); vertexIndex++)
	{
		int32_t nextVertexIndex = (vertexIndex + 1) % inLinesPoints.size();
		DrawDebugLine({ inLinesPoints[vertexIndex], inLinesPoints[nextVertexIndex], inColor });
	}
}

// Relies on vertex order:
//   0: Near Top Right
//   1: Near Bottom Right
//   2: Near Top Left
//   3: Near Bottom Left
//   4: Far Top Right
//   5: Far Bottom Right
//   6: Far Top Left
//   7: Far Bottom Left
 void DrawDebugHelpers::DrawBoxArray(eastl::array<glm::vec3, 8> inArray, const glm::vec3& inColor)
 {
 	int32_t FaceVertexIndices[6][4] =
 	{
 		{2, 3, 1, 0}, // Near face
 		{0, 1, 5, 4}, // Right face
 		{4, 5, 7, 6}, // Far face
 		{6, 7, 3, 2}, // Left face
 		{6, 2, 0, 4}, // Top face
 		{3, 7, 5, 1} // Bottom face
 	};


 	glm::vec3 FaceIndicatorColors[6] =
 	{
 		glm::vec3(1.f, 0.f, 0.f), // Red
 		glm::vec3(0.f, 1.f, 0.f), // Green
 		glm::vec3(1.f, 0.5f, 0.f), // Orange
 		glm::vec3(0.f, 0.5f, 0.4f), // Emerald
 		glm::vec3(0.f, 0.f, 1.f), // Blue
 		glm::vec3(0.f, 0.5f, 1.f), // Cyan
 	};

	eastl::array<glm::vec3, 4> FaceVertices;
 	for (int32_t faceCount = 0; faceCount < 6; faceCount++)
 	{
 		FaceVertices[0] = inArray[FaceVertexIndices[faceCount][0]];
 		FaceVertices[1] = inArray[FaceVertexIndices[faceCount][1]];
 		FaceVertices[2] = inArray[FaceVertexIndices[faceCount][2]];
 		FaceVertices[3] = inArray[FaceVertexIndices[faceCount][3]];
 
        eastl::vector<glm::vec3> verticesVector = FaceVertices.toVector();

		DrawLinesArray(verticesVector, inColor);
 
  		const glm::vec3 FaceCentre = (FaceVertices[0] + FaceVertices[1] + FaceVertices[2] + FaceVertices[3]) * 0.25f;
  		FaceVertices[0] = FaceCentre + ((FaceVertices[0] - FaceCentre) * 0.75f);
  		FaceVertices[1] = FaceCentre + ((FaceVertices[1] - FaceCentre) * 0.75f);
  		FaceVertices[2] = FaceCentre + ((FaceVertices[2] - FaceCentre) * 0.75f);
  		FaceVertices[3] = FaceCentre + ((FaceVertices[3] - FaceCentre) * 0.75f);
  
		verticesVector = FaceVertices.toVector();
 		DrawLinesArray(verticesVector, FaceIndicatorColors[faceCount]);
 	}
 }

void DrawDebugHelpers::DrawProjection(const glm::mat4& inProj)
{
	eastl::array<glm::vec3, 8> projCorners = RenderUtils::GenerateSpaceCorners(inProj, 0.f, 1.f);
	DrawBoxArray(projCorners, glm::vec3(1.f, 0.f, 0.f));
}

void DrawDebugManager::AddDebugPoint(const glm::vec3& inPoint)
{
	DebugPoints.push_back(inPoint);
}

void DrawDebugManager::AddDebugLine(const DebugLine& inLine)
{
	DebugLines.push_back(inLine);
}