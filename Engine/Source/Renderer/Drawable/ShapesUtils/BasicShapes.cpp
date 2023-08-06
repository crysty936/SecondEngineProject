#include "BasicShapes.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"
#include "Renderer/RHI/Resources/MeshDataContainer.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Core/SceneHelper.h"
#include "Renderer/Material/EngineMaterials/SkyboxMaterial.h"
#include "glad/glad.h"
#include "Renderer/RHI/Resources/RHIVertexBuffer.h"
#include "Renderer/RHI/RHI.h"
#include "Renderer/RHI/Resources/RHIIndexBuffer.h"
#include "Renderer/Material/EngineMaterials/BallTestMaterial.h"
#include "Renderer/Material/EngineMaterials/RenderMaterial_WithShadow.h"
#include "EASTL/unordered_map.h"
#include "Renderer/Material/EngineMaterials/RenderMaterial_LightSource.h"
#include "Renderer/Material/EngineMaterials/RenderMaterial_DeferredDecal.h"

TriangleShape::TriangleShape(const eastl::string& inName)
	: DrawableObject(inName)
{

}
TriangleShape::~TriangleShape() = default;

void TriangleShape::CreateProxy()
{
	const eastl::string RenderDataContainerID = "triangleVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Tex Coords
	//layout.Push<float>(2);

	if (!existingContainer)
	{
 		const int32_t indicesCount = BasicShapesData::GetTriangleIndicesCount();

		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(BasicShapesData::GetTriangleIndices(), indicesCount);


 		const int32_t verticesCount = BasicShapesData::GetTriangleVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, BasicShapesData::GetTriangleVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial("triangle_material", materialExists);

	if (!materialExists)
	{
		//eastl::string texturePath = "../Data/Textures/openGLExampleTransparentWindow.png";
		//eastl::shared_ptr<OpenGLTexture> tex = eastl::make_shared<OpenGLTexture>("DiffuseMap");
		//tex->Init(texturePath);
		//material->Textures.push_back(std::move(tex));

		eastl::vector<ShaderSourceInput> shaders = {
		{ "ModelWorldPositionVertexShader", EShaderType::Sh_Vertex },
		{ "FlatColorPixelShader", EShaderType::Sh_Fragment } };
		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = this_shared(this);
	newCommand.DrawType = EDrawType::DrawElements;

	Renderer::Get().AddCommand(newCommand);
}


SquareShape::SquareShape(const eastl::string& inName)
	: DrawableObject(inName)
{

}
SquareShape::~SquareShape() = default;

void SquareShape::CreateProxy()
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
 	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial>("square_material", materialExists);
 
 	if (!materialExists)
 	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "VS_Pos-UV", EShaderType::Sh_Vertex },
		//{ "GeometryTest_GS", EShaderType::Geometry },
		{ "PS_FlatColor", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}
 
 	RenderCommand newCommand;
 	newCommand.Material = material;
 	newCommand.DataContainer = dataContainer;
 	newCommand.Parent = this_shared(this);
 	newCommand.DrawType = EDrawType::DrawElements;
 
	Renderer::Get().AddCommand(newCommand);
}

CubeShape::CubeShape(const eastl::string& inName)
	: Model3D(inName)
{

}
CubeShape::~CubeShape() = default;

void CubeShape::CreateProxy()
{
  	const eastl::string RenderDataContainerID = "cubeVAO";
 	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);
	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
    // Vertex Normal
	inputLayout.Push<float>(3, VertexInputType::Normal);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);

 	if (!existingContainer)
  	{
		int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(BasicShapesData::GetCubeIndices(), indicesCount);


		int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, BasicShapesData::GetCubeVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
  	}
  
  	MaterialsManager& matManager = MaterialsManager::Get();
  	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial>("cube_material", materialExists);
  
  	if (!materialExists)
  	{
		eastl::shared_ptr<RHITexture2D> tex = RHI::Get()->CreateAndLoadTexture2D("../Data/Textures/MinecraftGrass.jpg", true);
		material->OwnedTextures.push_back(tex);

		eastl::vector<ShaderSourceInput> shaders = {
		{ "DefaultCubeMaterial/VS_Pos-UV-Normal_WithShadow", EShaderType::Sh_Vertex },
		{ "DefaultCubeMaterial/PS_BasicTex_WithShadow", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
  	}
  
  	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>("Cube Mesh");
  	AddChild(cubeNode);
  
	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = cubeNode;
	newCommand.DrawType = EDrawType::DrawElements;

	Renderer::Get().AddCommand(newCommand);
}

LightSource::LightSource(const eastl::string& inName)
	: Model3D(inName)
{

}
LightSource::~LightSource() = default;

void LightSource::CreateProxy()
{
	SceneManager::Get().GetCurrentScene().AddLight(this_shared(this));

	const eastl::string RenderDataContainerID = "lightVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);
	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Normal
	inputLayout.Push<float>(3, VertexInputType::Normal);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);

	if (!existingContainer)
	{
		int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(BasicShapesData::GetCubeIndices(), indicesCount);


		int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, BasicShapesData::GetCubeVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial_LightSource> material = matManager.GetOrAddMaterial<RenderMaterial_LightSource>("light_material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "LightSource/VS_Pos-UV-Normal", EShaderType::Sh_Vertex },
		{ "LightSource/PS_LightSource", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	// TODO: Hack, fix the Drawable, DrawableContainer stuff thing to allow this to work smoothly and
	// allow the uniform data to be extracted easily
	class LightMeshNode : public MeshNode
	{
	public:
		LightMeshNode(const eastl::string& inName, eastl::shared_ptr<LightSource>& inParent)
			: MeshNode(inName), Parent(inParent) {}

		virtual ~LightMeshNode() = default;


		void UpdateCustomUniforms(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache) const override
		{
			if (eastl::shared_ptr<LightSource> parentLocked = Parent.lock())
			{
				const LightData& lData = parentLocked->LData;
				inUniformsCache["LightColor"] = lData.Color;
			}
		}

		eastl::weak_ptr<LightSource> Parent;
	};


	eastl::shared_ptr<LightMeshNode> cubeNode = eastl::make_shared<LightMeshNode>("Light Mesh", this_shared(this));
	AddChild(cubeNode);

	material->bCastShadow = false;

	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = cubeNode;
	newCommand.DrawType = EDrawType::DrawElements;

	Renderer::Get().AddCommand(newCommand);
}

DeferredDecal::DeferredDecal(const eastl::string& inName)
	: Model3D(inName)
{

}
DeferredDecal::~DeferredDecal() = default;

void DeferredDecal::CreateProxy()
{
	const eastl::string RenderDataContainerID = "deferredDecalVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = Renderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);
	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Normal
	inputLayout.Push<float>(3, VertexInputType::Normal);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);

	if (!existingContainer)
	{
		int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(BasicShapesData::GetCubeIndices(), indicesCount);


		int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, BasicShapesData::GetCubeVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_DeferredDecal>("deferred_decal_material", materialExists);
	material->bUsesSceneTextures = true;

	if (!materialExists)
	{
		eastl::shared_ptr<RHITexture2D> tex = RHI::Get()->CreateAndLoadTexture2D("../Data/Textures/MinecraftGrass.jpg", true);
		material->OwnedTextures.push_back(tex);

		eastl::vector<ShaderSourceInput> shaders = {
		{ "DeferredDecal/VS_DeferredDecal", EShaderType::Sh_Vertex },
		{ "DeferredDecal/PS_DeferredDecal", EShaderType::Sh_Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>("Cube Mesh");
	AddChild(cubeNode);

	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = cubeNode;
	newCommand.DrawType = EDrawType::DrawElements;

	Renderer::Get().AddDecalCommand(newCommand);
}

