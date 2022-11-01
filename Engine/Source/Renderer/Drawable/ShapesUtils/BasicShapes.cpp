#include "BasicShapes.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"
#include "Renderer/RHI/Resources/RenderDataContainer.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/OpenGL/OpenGLCubeMap.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/OpenGL/RenderCommand.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Core/ObjectCreation.h"
#include "Renderer/Drawable/SkyboxMaterial.h"
#include "Renderer/Drawable/WithShadowMaterial.h"
#include "glad/glad.h"
#include "Renderer/RHI/Resources/VertexBufferBase.h"
#include "Renderer/RHI/RHI.h"
#include "Renderer/RHI/Resources/IndexBufferBase.h"

TriangleShape::TriangleShape() = default;
TriangleShape::~TriangleShape() = default;

void TriangleShape::CreateProxy()
{
	const eastl::string vaoName = "triangleVAO";
	eastl::shared_ptr<RenderDataContainer> dataContainer{ nullptr };

	const bool existingVAO = OpenGLRenderer::GetRHI().GetOrCreateContainer(vaoName, dataContainer);
	//const bool existingVAO = true; 

	if (!existingVAO)
	{
 		const int32_t indicesCount = BasicShapesData::GetTriangleIndicesCount();

		eastl::shared_ptr<IndexBufferBase> ib = RHI::Instance->CreateIndexBuffer(BasicShapesData::GetTriangleIndices(), indicesCount);

		VertexBufferLayout layout;
		// Vertex points
		layout.Push<float>(3);
		// Vertex Tex Coords
		//layout.Push<float>(2);

 		const int32_t verticesCount = BasicShapesData::GetTriangleVerticesCount();
		const eastl::shared_ptr<VertexBufferBase> vb = RHI::Instance->CreateVertexBuffer(layout, BasicShapesData::GetTriangleVertices(), verticesCount, ib);

		dataContainer->VBuffer = vb;
	}

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial("square_material", materialExists);

	if (!materialExists)
	{
		//eastl::string texturePath = "../Data/Textures/openGLExampleTransparentWindow.png";
		//eastl::shared_ptr<OpenGLTexture> tex = eastl::make_shared<OpenGLTexture>("DiffuseMap");
		//tex->Init(texturePath);
		//cubeMaterial->Textures.push_back(std::move(tex));

		//eastl::shared_ptr<ShaderBase> shader = RHIBase::RHI->CreateShaderFromPath("../Data/Shaders/OpenGL/BasicProjectionVertexShader.glsl", "../Data/Shaders/OpenGL/FragmentShader_ColorBasedOnPosition.glsl");

		cubeMaterial->Shader = RHI::Instance->CreateShaderFromPath("../Data/Shaders/OpenGL/BasicProjectionVertexShader.glsl", "../Data/Shaders/OpenGL/FragmentShader_ColorBasedOnPosition.glsl");
	}

	RenderCommand newCommand;
	newCommand.Material = cubeMaterial;
	newCommand.DataContainer = dataContainer;
	newCommand.Parent = this_shared(this);
	newCommand.DrawType = EDrawCallType::DrawElements;

	OpenGLRenderer::GetRHI().AddCommand(newCommand);
}


SquareShape::SquareShape() = default;
SquareShape::~SquareShape() = default;

void SquareShape::CreateProxy()
{
// 	const eastl::string vaoName = "squareVAO";
// 	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
// 	ASSERT(false); // Not working with Generic renderer
// 	//const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO); 
// 	const bool existingVAO = false;
// 
// 	if (!existingVAO)
// 	{
// 		// TODO: Buffers creation should be delegated to the renderer
// 		OpenGLIndexBuffer ibo = OpenGLIndexBuffer{};
// 		int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
// 		ibo.SetIndices(BasicShapesData::GetSquareIndices(), indicesCount);
// 
// 		VertexBufferLayout layout;
// 		// Vertex points
// 		layout.Push<float>(3);
// 		// Vertex Tex Coords
// 		layout.Push<float>(2);
// 
// 		OpenGLVertexBuffer vbo = OpenGLVertexBuffer{ ibo, layout };
// 		int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
// 		vbo.SetData(BasicShapesData::GetSquareVertices(), verticesCount);
// 
// 		//thisVAO->VBuffer = vbo;// TODO
// 	}
// 
// 	MaterialsManager& matManager = MaterialsManager::Get();
// 	bool materialExists = false;
// 	eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial("square_material", materialExists);
// 
// 	if (!materialExists)
// 	{
// 		//eastl::string texturePath = "../Data/Textures/openGLExampleTransparentWindow.png";
// 		//eastl::shared_ptr<OpenGLTexture> tex = eastl::make_shared<OpenGLTexture>("DiffuseMap");
// 		//tex->Init(texturePath);
// 		//cubeMaterial->Textures.push_back(std::move(tex));
// 		cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
// 	}
// 
// 	RenderCommand newCommand;
// 	newCommand.Material = cubeMaterial;
// 	newCommand.VAO = thisVAO;
// 	newCommand.Parent = this_shared(this);
// 	newCommand.DrawType = EDrawCallType::DrawElements;
// 
// 	ASSERT(false); // Not working with Generic renderer
// 	//RHI->AddCommand(newCommand);
}

eastl::shared_ptr<SquareShape> BasicShapes::CreateSquareObject(eastl::string inTexturePath)
{
// 	if (inTexturePath.empty())
// 	{
// 		inTexturePath = eastl::string("../Data/Textures/ExampleContainer.jpg");
// 	}

	eastl::shared_ptr<SquareShape> obj = ObjectCreation::NewObject<SquareShape>();

	return obj;
}

eastl::shared_ptr<CubeShape> BasicShapes::CreateCubeObject()
{
	eastl::shared_ptr<CubeShape> obj = ObjectCreation::NewObject<CubeShape>();

	return obj;
}

CubeShape::CubeShape() = default;
CubeShape::~CubeShape() = default;

void CubeShape::CreateProxy()
{
// 	const eastl::string vaoName = "cubeVAO";
// 	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
// 
// 	//TODO: Make work with generic renderer
// 	const bool existingVAO = OpenGLRenderer::GetRHI().GetOrCreateVAO(vaoName, thisVAO); 
// 	if (!existingVAO)
// 	{
// 		// TODO: Buffers creation should be delegated to the renderer
// 		OpenGLIndexBuffer ibo = OpenGLIndexBuffer{};
// 		int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
// 		ibo.SetIndices(BasicShapesData::GetCubeIndices(), indicesCount);
// 
// 		VertexBufferLayout layout;
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
// 	//eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial<WithShadowMaterial>("cube_material", materialExists);
// 	// TODO: Shadow disabled for now
// 	eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial<RenderMaterial>("cube_material", materialExists);
// 
// 	if (!materialExists)
// 	{
// 		eastl::string texturePath = "../Data/Textures/MinecraftGrass.jpg";
// 		eastl::shared_ptr<OpenGLTexture> tex = eastl::make_shared<OpenGLTexture>("DiffuseMap");
// 		tex->Init(texturePath);
// 		cubeMaterial->Textures.push_back(std::move(tex));
// 		cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicPerspVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
// 		//cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/WithNormalProjectionVertexShader.glsl", "../Data/Shaders/LightingTexFragmentShader.glsl");
// 	}
// 
// 	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>();
// 	AddChild(cubeNode);
// 
// 	RenderCommand newCommand;
// 	newCommand.Material = cubeMaterial;
// 	newCommand.VAO = thisVAO;
// 	newCommand.Parent = cubeNode;
// 	newCommand.DrawType = EDrawCallType::DrawElements;
// 
// 	//TODO: Make work with generic renderer
// 	OpenGLRenderer::GetRHI().AddCommand(newCommand);
}

Skybox::Skybox() = default;
Skybox::~Skybox() = default;

void Skybox::CreateProxy()
{
// 	const eastl::string vaoName = "skyboxVAO";
// 	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
// 
// 	ASSERT(false); // Not working with Generic renderer
// 	//const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO); 
// 	const bool existingVAO = false;
// 
// 	if (!existingVAO)
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
// 	const eastl::string vaoName = "lightSourceVAO";
// 	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
// 	ASSERT(false); // Not working with Generic renderer
// 	//const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO);
// 	const bool existingVAO = false;
// 
// 	if (!existingVAO)
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
