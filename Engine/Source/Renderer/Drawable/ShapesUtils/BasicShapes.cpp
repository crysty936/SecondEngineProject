#include "BasicShapes.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"
#include "Renderer/RHI/Resources/MeshDataContainer.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/ForwardRenderer.h"
#include "Core/EntityHelper.h"
#include "Renderer/Material/EngineMaterials/SkyboxMaterial.h"
#include "glad/glad.h"
#include "Renderer/RHI/Resources/RHIVertexBuffer.h"
#include "Renderer/RHI/RHI.h"
#include "Renderer/RHI/Resources/RHIIndexBuffer.h"
#include "Renderer/Material/EngineMaterials/BallTestMaterial.h"
#include "Renderer/Material/EngineMaterials/RenderMaterial_WithShadow.h"
#include "EASTL/unordered_map.h"
#include "Renderer/Material/EngineMaterials/RenderMaterial_LightSource.h"

TriangleShape::TriangleShape(const eastl::string& inName)
	: DrawableObject(inName)
{

}
TriangleShape::~TriangleShape() = default;

void TriangleShape::CreateProxy()
{
	const eastl::string RenderDataContainerID = "triangleVAO";
	eastl::shared_ptr<MeshDataContainer> dataContainer{ nullptr };

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

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
		{ "ModelWorldPositionVertexShader", EShaderType::Vertex },
		{ "FlatColorPixelShader", EShaderType::Fragment } };
		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = this_shared(this);
	newCommand.DrawType = EDrawType::DrawElements;

	ForwardRenderer::Get().AddCommand(newCommand);
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
 	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial>("square_material", materialExists);
 
 	if (!materialExists)
 	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "VS_Pos-UV", EShaderType::Vertex },
		//{ "GeometryTest_GS", EShaderType::Geometry },
		{ "PS_FlatColor", EShaderType::Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}
 
 	RenderCommand newCommand;
 	newCommand.Material = material;
 	newCommand.DataContainer = dataContainer;
 	newCommand.Parent = this_shared(this);
 	newCommand.DrawType = EDrawType::DrawElements;
 
	ForwardRenderer::Get().AddCommand(newCommand);
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

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);
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
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_WithShadow>("cube_material", materialExists);
  
  	if (!materialExists)
  	{
		eastl::shared_ptr<RHITexture2D> tex = RHI::Get()->CreateTexture2D("../Data/Textures/MinecraftGrass.jpg");
		material->OwnedTextures.push_back(tex);

		eastl::vector<ShaderSourceInput> shaders = {
		{ "VS_Pos-UV-Normal_WithShadow", EShaderType::Vertex },
		{ "PS_BasicTex_WithShadow", EShaderType::Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
  	}
  
  	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>("Cube Mesh");
  	AddChild(cubeNode);
  
	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = cubeNode;
	newCommand.DrawType = EDrawType::DrawElements;

	ForwardRenderer::Get().AddCommand(newCommand);
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

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);
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
		{ "LightSource/VS_Pos-UV-Normal", EShaderType::Vertex },
		{ "LightSource/PS_LightSource", EShaderType::Fragment } };

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

	ForwardRenderer::Get().AddCommand(newCommand);
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

class FullScreenQuadMaterial : public RenderMaterial
{

public:
	virtual void SetRequiredUniforms() override
	{}
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
	eastl::shared_ptr<FullScreenQuadMaterial> material = matManager.GetOrAddMaterial<FullScreenQuadMaterial>("TexQuad_Material", materialExists);

	if (!materialExists)
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "Pos-UV_BasicTex/VS_Pos-UV_UnchangedPosition", EShaderType::Vertex },
		{ "Pos-UV_BasicTex/PS_BasicTex", EShaderType::Fragment } };

		material->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
	}

	QuadCommand.Material = material;
	QuadCommand.DataContainer = dataContainer;
	QuadCommand.Parent = this_shared(this);
	QuadCommand.DrawType = EDrawType::DrawElements;
}

