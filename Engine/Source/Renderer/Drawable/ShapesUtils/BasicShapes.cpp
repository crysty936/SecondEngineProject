#include "BasicShapes.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"
#include "Renderer/RHI/Resources/RenderDataContainer.h"
#include "Renderer/OpenGL/OpenGLCubeMap.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/ForwardRenderer.h"
#include "Core/EntityHelper.h"
#include "Renderer/Drawable/SkyboxMaterial.h"
#include "Renderer/Drawable/WithShadowMaterial.h"
#include "glad/glad.h"
#include "Renderer/RHI/Resources/RHIVertexBuffer.h"
#include "Renderer/RHI/RHI.h"
#include "Renderer/RHI/Resources/RHIIndexBuffer.h"
#include "Renderer/Drawable//BallTestMaterial.h"

TriangleShape::TriangleShape() = default;
TriangleShape::~TriangleShape() = default;

void TriangleShape::CreateProxy()
{
	const eastl::string RenderDataContainerID = "triangleVAO";
	eastl::shared_ptr<RenderDataContainer> dataContainer{ nullptr };

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);

	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Tex Coords
	//layout.Push<float>(2);

	if (!existingContainer)
	{
 		const int32_t indicesCount = BasicShapesData::GetTriangleIndicesCount();

		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Instance->CreateIndexBuffer(BasicShapesData::GetTriangleIndices(), indicesCount);


 		const int32_t verticesCount = BasicShapesData::GetTriangleVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Instance->CreateVertexBuffer(inputLayout, BasicShapesData::GetTriangleVertices(), verticesCount, ib);

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
		material->Shader = RHI::Instance->CreateShaderFromPath(shaders, inputLayout);
	}

	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = this_shared(this);
	newCommand.DrawType = EDrawCallType::DrawElements;

	ForwardRenderer::Get().AddCommand(newCommand);
}


SquareShape::SquareShape() = default;
SquareShape::~SquareShape() = default;

void SquareShape::CreateProxy()
{
 	const eastl::string RenderDataContainerID = "squareVAO";
	eastl::shared_ptr<RenderDataContainer> dataContainer{ nullptr };

	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);
 
 	VertexInputLayout inputLayout;
 	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Vertex Tex Coords
 	inputLayout.Push<float>(2, VertexInputType::TexCoords);

	if (!existingContainer)
 	{
 		int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Instance->CreateIndexBuffer(BasicShapesData::GetSquareIndices(), indicesCount);

 
 		int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Instance->CreateVertexBuffer(inputLayout, BasicShapesData::GetSquareVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}
 
 	MaterialsManager& matManager = MaterialsManager::Get();
 	bool materialExists = false;
 	eastl::shared_ptr<BallTestMaterial> material = matManager.GetOrAddMaterial<BallTestMaterial>("square_material", materialExists);
 
 	if (!materialExists)
 	{
  		eastl::shared_ptr<RHITexture2D> tex = RHI::Instance->CreateTexture2D("../Data/Textures/numbers_corrected.png");
  		material->DiffuseTextures.push_back(tex);

		eastl::vector<ShaderSourceInput> shaders = {
		{ "ModelWorldPosition_VS_Pos-UV", EShaderType::Vertex },
		//{ "GeometryTest_GS", EShaderType::Geometry },
		{ "FlatColor_PS", EShaderType::Fragment } };

		material->Shader = RHI::Instance->CreateShaderFromPath(shaders, inputLayout);
	}
 
 	RenderCommand newCommand;
 	newCommand.Material = material;
 	newCommand.DataContainer = dataContainer;
 	newCommand.Parent = this_shared(this);
 	newCommand.DrawType = EDrawCallType::DrawElements;
 
	ForwardRenderer::Get().AddCommand(newCommand);
}

eastl::shared_ptr<SquareShape> BasicShapes::CreateSquareObject(eastl::string inTexturePath)
{
	eastl::shared_ptr<SquareShape> obj = EntityHelper::CreateObject<SquareShape>();
	return obj;
}

eastl::shared_ptr<CubeShape> BasicShapes::CreateCubeObject()
{
	eastl::shared_ptr<CubeShape> obj = EntityHelper::CreateObject<CubeShape>();
	return obj;
}

CubeShape::CubeShape() = default;
CubeShape::~CubeShape() = default;

void CubeShape::CreateProxy()
{
  	const eastl::string RenderDataContainerID = "cubeVAO";
 	eastl::shared_ptr<RenderDataContainer> dataContainer{ nullptr };

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
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Instance->CreateIndexBuffer(BasicShapesData::GetCubeIndices(), indicesCount);


		int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Instance->CreateVertexBuffer(inputLayout, BasicShapesData::GetCubeVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
  	}
  
  	MaterialsManager& matManager = MaterialsManager::Get();
  	bool materialExists = false;
  	//eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial<WithShadowMaterial>("cube_material", materialExists);
  	// TODO: Shadow disabled for now
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial("cube_material", materialExists);
  
  	if (!materialExists)
  	{
		eastl::shared_ptr<RHITexture2D> tex = RHI::Instance->CreateTexture2D("../Data/Textures/MinecraftGrass.jpg");
		material->DiffuseTextures.push_back(tex);

		eastl::vector<ShaderSourceInput> shaders = {
		{ "ModelWorldPosition_VS_Pos-UV-Normal_ManuallyWritten", EShaderType::Vertex },
		{ "BasicTex_PS", EShaderType::Fragment } };

		material->Shader = RHI::Instance->CreateShaderFromPath(shaders, inputLayout);
  	}
  
  	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>();
  	AddChild(cubeNode);
  
	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = cubeNode;
	newCommand.DrawType = EDrawCallType::DrawElements;

	ForwardRenderer::Get().AddCommand(newCommand);
}

Skybox::Skybox() = default;
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

LightSource::LightSource() = default;
LightSource::~LightSource() = default;

void LightSource::CreateProxy()
{
// 	const eastl::string RenderDataContainerID = "lightSourceVAO";
// 	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
// 	ASSERT(false); // Not working with Generic renderer
// 	//const bool existingContainer = RHI->GetOrCreateVAO(RenderDataContainerID, thisVAO);
// 	const bool existingContainer = false;
// 
// 	if (!existingContainer)
// 	{
// 		// TODO: Buffers creation should be delegated to the renderer
// 		OpenGLIndexBuffer ibo = OpenGLIndexBuffer{};
// 		int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
// 		ibo.SetIndices(BasicShapesData::GetCubeIndices(), indicesCount);
// 
// 		VertexBufferLayout layout = VertexBufferLayout{};
// 		// Vertex points
// 		layout.Push<float>(3);
// 		// Normals
// 		layout.Push<float>(3);
// 		// Vertex Tex Coords
// 		layout.Push<float>(2);
// 
// 		OpenGLVertexBuffer vbo = OpenGLVertexBuffer{ ibo, layout };
// 		int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
// 		vbo.SetData(BasicShapesData::GetCubeVertices(), verticesCount);
// 
// 		//thisVAO->VBuffer = vbo;// TODO
// 	}
// 
// 	MaterialsManager& matManager = MaterialsManager::Get();
// 	bool materialExists = false;
// 	eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial<RenderMaterial>("light_source_material", materialExists);
// 
// 	if (!materialExists)
// 	{
// 		cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/WithNormalProjectionVertexShader.glsl", "../Data/Shaders/LightSourceFragmentShader.glsl");
// 	}
// 
// 	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>();
// 	AddChild(cubeNode);
// 
// 	RenderCommand newCommand;
// 	newCommand.Material = cubeMaterial;
// 	newCommand.VAO = thisVAO;
// 	newCommand.Parent = cubeNode;
// 	newCommand.DrawType = EDrawCallType::DrawArrays;
// 
// 	ASSERT(false); // Not working with Generic renderer
	//RHI->AddCommand(newCommand);
}
