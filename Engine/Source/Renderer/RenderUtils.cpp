#include "RenderUtils.h"
#include "EASTL/string.h"
#include "RHI/Resources/VertexInputLayout.h"
#include "Renderer.h"
#include "Drawable/ShapesUtils/BasicShapesData.h"
#include "RHI/RHI.h"
#include "Material/MaterialsManager.h"
#include "Material/RenderMaterial.h"
#include "Material/EngineMaterials/RenderMaterial_VisualizeDepth.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Material/EngineMaterials/RenderMaterial_LightSource.h"
#include "Material/EngineMaterials/RenderMaterial_DefaultLighting.h"

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



Skybox::Skybox(const eastl::string& inName)
	: Model3D(inName)
{

}
Skybox::~Skybox() = default;

void Skybox::CreateProxy()
{
	// 	const eastl::string RenderDataContainerID = "skyboxVAO";
	// 	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
	// 
	// 	ASSERT(false); // Not working with Generic renderer
	// 	//const bool existingContainer = RHI->GetOrCreateVAO(RenderDataContainerID, thisVAO); 
	// 	const bool existingContainer = false;
	// 
	// 	if (!existingContainer)
	// 	{
	// 		// TODO: Buffers creation should be delegated to the renderer
	// 		OpenGLIndexBuffer ibo = OpenGLIndexBuffer{};
	// 		ibo.IndicesCount = BasicShapesData::GetSkyboxIndicesCount();
	// 		VertexBufferLayout layout;
	// 		// Vertex points
	// 		layout.Push<float>(3);
	// 
	// 		OpenGLVertexBuffer vbo = OpenGLVertexBuffer{ ibo, layout };
	// 		int32_t verticesCount = BasicShapesData::GetSkyboxVerticesCount();
	// 		vbo.SetData(BasicShapesData::GetSkyboxVertices(), verticesCount);
	// 
	// 		//thisVAO->VBuffer = vbo;// TODO
	// 	}
	// 
	// 	MaterialsManager& matManager = MaterialsManager::Get();
	// 	bool materialExists = false;
	// 	eastl::shared_ptr<RenderMaterial> thisMaterial = matManager.GetOrAddMaterial<SkyboxMaterial>("skybox_material", materialExists);
	// 
	// 	if (!materialExists)
	// 	{
	// 		eastl::vector<eastl::string> textures
	// 		{
	// 			"../Data/Textures/skybox/right.jpg",
	// 			"../Data/Textures/skybox/left.jpg",
	// 			"../Data/Textures/skybox/top.jpg",
	// 			"../Data/Textures/skybox/bottom.jpg",
	// 			"../Data/Textures/skybox/front.jpg",
	// 			"../Data/Textures/skybox/back.jpg",
	// 		};
	// 
	// 		eastl::shared_ptr<OpenGLCubeMap> tex = eastl::make_shared<OpenGLCubeMap>("CubeMap");
	// 		tex->Init(textures);
	// 		thisMaterial->Textures.push_back(std::move(tex));
	// 		thisMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/SkyboxVertexShader.glsl", "../Data/Shaders/SkyboxFragmentShader.glsl");
	// 	}
	// 
	// 	eastl::shared_ptr<MeshNode> thisNode = eastl::make_shared<MeshNode>();
	// 	AddChild(thisNode);
	// 
	// 	RenderCommand newCommand;
	// 	newCommand.Material = thisMaterial;
	// 	newCommand.VAO = thisVAO;
	// 	newCommand.Parent = thisNode;
	// 	newCommand.DrawType = EDrawCallType::DrawArrays;
	// 
	// 	ASSERT(false); // Not working with Generic renderer
		//RHI->SetSkyboxCommand(newCommand);
}

// Mirror

MirrorQuad::MirrorQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}
MirrorQuad::~MirrorQuad() = default;

void MirrorQuad::CreateProxy()
{
	// 	const eastl::string vaoName = "mirrorVAO";
	// 	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
	// 	ASSERT(false); // Not working with Generic renderer
	// 	//const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO); 
	// 	const bool existingVAO = false;
	// 
	// 	if (!existingVAO)
	// 	{
	// 		// TODO: Buffers creation should be delegated to the renderer
	// 		OpenGLIndexBuffer ibo = OpenGLIndexBuffer{};
	// 		int32_t indicesCount = BasicShapesData::GetQuadIndicesCount();
	// 		ibo.SetIndices(BasicShapesData::GetQuadIndices(), indicesCount);
	// 
	// 		VertexBufferLayout layout = VertexBufferLayout{};
	// 		// Vertex points
	// 		layout.Push<float>(3);
	// 		// Vertex Tex Coords
	// 		layout.Push<float>(2);
	// 
	// 		OpenGLVertexBuffer vbo = OpenGLVertexBuffer{ ibo, layout };
	// 		int32_t verticesCount = BasicShapesData::GetQuadVerticesCount();
	// 		vbo.SetData(BasicShapesData::GetQuadVertices(), verticesCount);
	// 
	// 		//thisVAO->VBuffer = vbo;// TODO
	// 	}
	// 	// Mirrors have to have unique materials
	// 	static int mirrorNr = 0;
	// 	const eastl::string mirrorMaterialName(eastl::string::CtorSprintf{}, "mirror_material_%d", ++mirrorNr);
	// 
	// 	MaterialsManager& matManager = MaterialsManager::Get();
	// 	bool materialExists = false;
	// 	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial(mirrorMaterialName, materialExists);
	// 	//eastl::string texturePath = "../Data/Textures/openGLExampleTransparentWindow.png";
	// 
	// 	if (!materialExists)
	// 	{
	// 		//OpenGLTexture tex{ texturePath, texureBaseNr + 0 };
	// 		//cubeMaterial->Textures.push_back(tex);
	// 		material->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/QuadTexFragmentShader.glsl");
	// 	}
	// 
	// 	//material->Textures.push_back(OpenGLTexture());
	// 
	// 	RenderCommand newCommand;
	// 	newCommand.Material = material;
	// 	newCommand.VAO = thisVAO;
	// 	newCommand.Parent = this_shared(this);
	// 	newCommand.DrawType = EDrawCallType::DrawElements;
	// 
	// 	//RHI->AddMirrorCommand(newCommand);

}

class GBufferVisualizeMaterial : public RenderMaterial
{

public:
	virtual void SetRequiredUniforms() override
	{
 		eastl::vector<UniformWithFlag> defaultUniforms = {
 			{"model"}
 		};
 
 		UBuffers.push_back({ defaultUniforms, EShaderType::Sh_Vertex });
	}
};

FullScreenQuad::FullScreenQuad(const eastl::string& inName)
	: DrawableObject(inName)
{
}
FullScreenQuad::~FullScreenQuad() = default;

void FullScreenQuad::CreateProxy()
{

}

void FullScreenQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial("ScreenQuad_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "ScreenQuad/VS_Pos-UV_UnchangedPosition", EShaderType::Sh_Vertex },
		{ "ScreenQuad/PS_BasicTex", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}


GBufferVisualizeQuad::GBufferVisualizeQuad(const eastl::string& inName)
	: DrawableObject(inName)
{
}
GBufferVisualizeQuad::~GBufferVisualizeQuad() = default;

void GBufferVisualizeQuad::CreateProxy()
{}

void GBufferVisualizeQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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
	eastl::shared_ptr<GBufferVisualizeMaterial> material = matManager.GetOrAddMaterial<GBufferVisualizeMaterial>("GbufferVisualize_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "GBufferVisualize/VS_ModelPosition-UV", EShaderType::Sh_Vertex },
		{ "GBufferVisualize/PS_BasicTex", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}



ToneMapQuad::ToneMapQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}

void ToneMapQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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

			UBuffers.push_back({ defaultUniforms, EShaderType::Sh_Fragment });
		}
	};

	bool materialExists = false;
	eastl::shared_ptr<TonemapMaterial> material = matManager.GetOrAddMaterial<TonemapMaterial>("TonemapQuad_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "ToneMapping/VS_Pos-UV_UnchangedPosition", EShaderType::Sh_Vertex },
		{ "ToneMapping/PS_ToneMap", EShaderType::Sh_Fragment } };

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

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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
		{ "Bloom/VS_Pos-UV_UnchangedPosition", EShaderType::Sh_Vertex },
		{ "Bloom/PS_ExtractBrightAreas", EShaderType::Sh_Fragment } };

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

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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

			UBuffers.push_back({ defaultUniforms, EShaderType::Sh_Fragment });
		}
	};


	bool materialExists = false;
	eastl::shared_ptr<BlurMaterial> material = matManager.GetOrAddMaterial<BlurMaterial>("GaussianBlur_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "Bloom/VS_Pos-UV_UnchangedPosition", EShaderType::Sh_Vertex },
		{ "Bloom/PS_GaussianBlur", EShaderType::Sh_Fragment } };

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

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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
		{ "Bloom/VS_Pos-UV_UnchangedPosition", EShaderType::Sh_Vertex },
		{ "Bloom/PS_MergeBlur", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}

VisualizeDepthQuad::VisualizeDepthQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}

void VisualizeDepthQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_VisualizeDepth>("VisualizeDepth_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "VisualizeDepth/VS_ModelPosition-UV", EShaderType::Sh_Vertex },
		{ "VisualizeDepth/PS_VisualiseDepth", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}


DefaultLightingModelQuad::DefaultLightingModelQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}

void DefaultLightingModelQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_DefaultLighting>("RenderMaterial_DefaultLighting", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "DefaultLightingModel/VS_Pos-UV_UnchangedPosition", EShaderType::Sh_Vertex },
		{ "DefaultLightingModel/PS_DefaultLightingModel", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}





DefaultPBRLightingModelQuad::DefaultPBRLightingModelQuad(const eastl::string& inName)
	: DrawableObject(inName)
{

}

void DefaultPBRLightingModelQuad::CreateCommand()
{
	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_DefaultLighting>("RenderMaterial_DefaultLighting", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "DefaultPBRLightingModel/VS_Pos-UV_UnchangedPosition", EShaderType::Sh_Vertex },
		{ "DefaultPBRLightingModel/PS_DefaultLightingModel", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}




