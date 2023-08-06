#include "DrawDebugHelpers.h"
#include "Utils/InlineVector.h"
#include "Renderer.h"
#include "RenderUtils.h"
#include "RHI/RHI.h"
#include "Material/MaterialsManager.h"
#include "Material/EngineMaterials/RenderMaterial_Debug.h"

eastl::shared_ptr<RHIVertexBuffer> DebugPointsBuffer = nullptr;
eastl::shared_ptr<RHIVertexBuffer> DebugLinesBuffer = nullptr;

void DrawDebugHelpers::DrawDebugPoint(const glm::vec3 inPoint)
{
	DrawDebugManager::Get().AddDebugPoint(inPoint);
}

void DrawDebugHelpers::DrawDebugLine(const DebugLine& inLine)
{
	DrawDebugManager::Get().AddDebugLine(inLine);
}

void DrawDebugHelpers::DrawDebugLine(const glm::vec3& inStart, const glm::vec3& inEnd, const glm::vec3& inColor)
{
	DrawDebugLine({inStart, inEnd, inColor});
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
 void DrawDebugHelpers::DrawBoxArray(eastl::array<glm::vec3, 8> inArray, const bool inDrawSides, const glm::vec3& inColor)
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
 
		if (inDrawSides)
		{
  			const glm::vec3 FaceCentre = (FaceVertices[0] + FaceVertices[1] + FaceVertices[2] + FaceVertices[3]) * 0.25f;
  			FaceVertices[0] = FaceCentre + ((FaceVertices[0] - FaceCentre) * 0.75f);
  			FaceVertices[1] = FaceCentre + ((FaceVertices[1] - FaceCentre) * 0.75f);
  			FaceVertices[2] = FaceCentre + ((FaceVertices[2] - FaceCentre) * 0.75f);
  			FaceVertices[3] = FaceCentre + ((FaceVertices[3] - FaceCentre) * 0.75f);
  
			verticesVector = FaceVertices.toVector();
 			DrawLinesArray(verticesVector, FaceIndicatorColors[faceCount]);
		}
 	}
 }

void DrawDebugHelpers::DrawProjection(const glm::mat4& inProj)
{
	eastl::array<glm::vec3, 8> projCorners = RenderUtils::GenerateSpaceCorners(inProj, 0.f, 1.f);
	DrawBoxArray(projCorners, true, glm::vec3(1.f, 0.f, 0.f));
}

void DrawDebugManager::Draw()
{
	// Points
	{
		const eastl::vector<glm::vec3> debugPoints = Get().DebugPoints;

		const int32_t numPoints = static_cast<int32_t>(debugPoints.size());

		VertexInputLayout inputLayout;
		// Vertex points
		inputLayout.Push<float>(3, VertexInputType::Position);

		const size_t pointsSize = sizeof(glm::vec3) * numPoints;

		if (!DebugPointsBuffer)
		{
			DebugPointsBuffer = RHI::Get()->CreateVertexBuffer(inputLayout, debugPoints.data(), pointsSize);
		}
		else
		{
			RHI::Get()->ClearVertexBuffer(*DebugPointsBuffer);
			RHI::Get()->UpdateVertexBufferData(*DebugPointsBuffer, debugPoints.data(), pointsSize);
		}

		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_Debug>("debug_points_material", materialExists);

		if (!materialExists)
		{
			eastl::vector<ShaderSourceInput> shaders = {
			{ "Debug/VS_Pos_ManuallyWritten_DebugPoints", EShaderType::Sh_Vertex },
			{ "Debug/PS_FlatColor", EShaderType::Sh_Fragment } };

			material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
		}

		constexpr bool useIndexBuffer = false;
		RHI::Get()->BindVertexBuffer(*DebugPointsBuffer, useIndexBuffer);
		RHI::Get()->BindShader(*material->Shader);

		material->ResetUniforms();

		material->SetUniformsValue(Renderer::Get().GetUniformsCache());
		material->BindBuffers();

		RHI::Get()->DrawPoints(numPoints);

		RHI::Get()->UnbindVertexBuffer(*DebugPointsBuffer, useIndexBuffer);
		material->UnbindBuffers();
		RHI::Get()->UnbindShader(*material->Shader);
	}

	// Lines
	{
		const eastl::vector<DebugLine> debugLines = Get().DebugLines;

		VertexInputLayout inputLayout;
		// Vertex points
		inputLayout.Push<float>(3, VertexInputType::Position);
		inputLayout.Push<float>(3, VertexInputType::Undefined);
		inputLayout.Push<float>(3, VertexInputType::Undefined);

		const size_t linesSize = sizeof(DebugLine) * debugLines.size();

		if (!DebugLinesBuffer)
		{
			DebugLinesBuffer = RHI::Get()->CreateVertexBuffer(inputLayout, debugLines.data(), linesSize);
		}
		else
		{
			RHI::Get()->ClearVertexBuffer(*DebugLinesBuffer);
			RHI::Get()->UpdateVertexBufferData(*DebugLinesBuffer, debugLines.data(), linesSize);
		}

		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_Debug>("debug_lines_material", materialExists);

		if (!materialExists)
		{
			eastl::vector<ShaderSourceInput> shaders = {
			{ "Debug/VS_Pos_Geometry_ManuallyWritten_DebugLine", EShaderType::Sh_Vertex },
			{ "Debug/GS_DebugLines", EShaderType::Sh_Geometry },
			{ "Debug/PS_DebugLine_Color", EShaderType::Sh_Fragment } };

			material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
		}

		constexpr bool useIndexBuffer = false;
		RHI::Get()->BindVertexBuffer(*DebugLinesBuffer, useIndexBuffer);
		RHI::Get()->BindShader(*material->Shader);

		material->ResetUniforms();

		material->SetUniformsValue(Renderer::Get().GetUniformsCache());
		material->BindBuffers();

		RHI::Get()->DrawPoints(static_cast<int32_t>(debugLines.size()));

		RHI::Get()->UnbindVertexBuffer(*DebugLinesBuffer, useIndexBuffer);
		material->UnbindBuffers();
		RHI::Get()->UnbindShader(*material->Shader);
	}

	Get().ClearDebugData();
}

void DrawDebugManager::AddDebugPoint(const glm::vec3& inPoint)
{
	DebugPoints.push_back(inPoint);
}

void DrawDebugManager::AddDebugLine(const DebugLine& inLine)
{
	DebugLines.push_back(inLine);
}
