#include "RenderUtils.h"
#include "EASTL/string.h"
#include "RHI/Resources/VertexInputLayout.h"
#include "ForwardRenderer.h"
#include "Drawable/ShapesUtils/BasicShapesData.h"
#include "RHI/RHI.h"
#include "Material/MaterialsManager.h"
#include "Material/RenderMaterial.h"

eastl::array<glm::vec3, 8> RenderUtils::GenerateSpaceCorners(const glm::mat4& SpaceToProjectionSpace, const float MinZ, const float MaxZ)
{
	const glm::vec3 ProjectionSpaceCorners[8] =
	{
		glm::vec3(1.0f, 1.0f, MinZ),
		glm::vec3(-1.0f, 1.0f, MinZ),
		glm::vec3(1.0f, -1.0f, MinZ),
		glm::vec3(-1.0f, -1.0f, MinZ),
		glm::vec3(1.0f, 1.0f, MaxZ),
		glm::vec3(-1.0f, 1.0f, MaxZ),
		glm::vec3(1.0f, -1.0f, MaxZ),
		glm::vec3(-1.0f, -1.0f, MaxZ)
	};

	const glm::mat4 ProjectionToSpace = glm::inverse(SpaceToProjectionSpace);
	eastl::array<glm::vec3, 8> SpaceCorners;

	for (int i = 0; i < 8; ++i)
	{
		const glm::vec3& currentCorner = ProjectionSpaceCorners[i];
		const glm::vec4 worldPos = ProjectionToSpace * glm::vec4(currentCorner.x, currentCorner.y, currentCorner.z, 1.f);
		SpaceCorners[i] = worldPos / worldPos.w;
	}

	return SpaceCorners;
}

glm::vec3 RenderUtils::GetProjectionCenter(const glm::mat4& inProj)
{
	eastl::array<glm::vec3, 8> projCorners = GenerateSpaceCorners(inProj);

	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const glm::vec3& v : projCorners)
	{
		center += glm::vec3(v);
	}
	center /= projCorners.size();

	return center;
}

ToneMapQuad::ToneMapQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}

void ToneMapQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);

	if (!existingContainer)
	{
		int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(BasicShapesData::GetSquareIndices(), indicesCount);


		int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, BasicShapesData::GetSquareVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}

	MaterialsManager& matManager = MaterialsManager::Get();

	class TonemapMaterial : public RenderMaterial
	{

	public:
		virtual void SetRequiredUniforms() override
		{
			eastl::vector<UniformWithFlag> defaultUniforms = {
			{"Exposure"},
			};

			UBuffers.push_back({ defaultUniforms, ConstantBufferBinding::Pixel });
		}
	};

	bool materialExists = false;
	eastl::shared_ptr<TonemapMaterial> material = matManager.GetOrAddMaterial<TonemapMaterial>("TonemapQuad_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "ToneMapping/VS_Pos-UV_UnchangedPosition", EShaderType::Vertex },
		{ "ToneMapping/PS_ToneMap", EShaderType::Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}

ExtractBrightAreasQuad::ExtractBrightAreasQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}

void ExtractBrightAreasQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);

	if (!existingContainer)
	{
		int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(BasicShapesData::GetSquareIndices(), indicesCount);


		int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, BasicShapesData::GetSquareVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}

	MaterialsManager& matManager = MaterialsManager::Get();

	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial>("ExtractBrightAreas_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "Bloom/VS_Pos-UV_UnchangedPosition", EShaderType::Vertex },
		{ "Bloom/PS_ExtractBrightAreas", EShaderType::Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}


GaussianBlurQuad::GaussianBlurQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}

void GaussianBlurQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);

	if (!existingContainer)
	{
		int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(BasicShapesData::GetSquareIndices(), indicesCount);


		int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, BasicShapesData::GetSquareVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}

	MaterialsManager& matManager = MaterialsManager::Get();

	class BlurMaterial : public RenderMaterial
	{

	public:
		virtual void SetRequiredUniforms() override
		{
			eastl::vector<UniformWithFlag> defaultUniforms = {
			{"BlurHorizontal"},
			};

			UBuffers.push_back({ defaultUniforms, ConstantBufferBinding::Pixel });
		}
	};


	bool materialExists = false;
	eastl::shared_ptr<BlurMaterial> material = matManager.GetOrAddMaterial<BlurMaterial>("GaussianBlur_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "Bloom/VS_Pos-UV_UnchangedPosition", EShaderType::Vertex },
		{ "Bloom/PS_GaussianBlur", EShaderType::Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}

BloomMergeQuad::BloomMergeQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}

void BloomMergeQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);

	if (!existingContainer)
	{
		int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(BasicShapesData::GetSquareIndices(), indicesCount);


		int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, BasicShapesData::GetSquareVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}

	MaterialsManager& matManager = MaterialsManager::Get();

	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial("MergeBloom_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "Bloom/VS_Pos-UV_UnchangedPosition", EShaderType::Vertex },
		{ "Bloom/PS_MergeBlur", EShaderType::Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}




