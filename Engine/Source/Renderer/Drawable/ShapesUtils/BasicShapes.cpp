#include "BasicShapes.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"
#include "Renderer/OpenGL/Buffer/IndexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexBufferLayout.h"
#include "Renderer/OpenGL/Buffer/VertexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexArrayObject.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/OpenGL/OpenGLCubeMap.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/OpenGL/RenderCommand.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Core/ObjectCreation.h"
#include "Renderer/Drawable/SkyboxMaterial.h"
#include "Renderer/Drawable/WithShadowMaterial.h"


TriangleShape::TriangleShape() = default;
TriangleShape::~TriangleShape() = default;

void TriangleShape::SetupDrawCommands()
{
	const eastl::string vaoName = "triangleVAO";
	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
	const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO);

	if (!existingVAO)
	{
		// TODO: Buffers creation should be delegated to the renderer
		IndexBuffer ibo = IndexBuffer{};
		int32_t indicesCount = BasicShapesData::GetTriangleIndicesCount();
		ibo.SetIndices(BasicShapesData::GetTriangleIndices(), indicesCount, GL_STATIC_DRAW);

		VertexBufferLayout layout = VertexBufferLayout{};
		// Vertex points
		layout.Push<float>(3);
		// Vertex Tex Coords
		layout.Push<float>(2);

		VertexBuffer vbo = VertexBuffer{ ibo, layout };
		int32_t verticesCount = BasicShapesData::GetTriangleVerticesCount();
		vbo.SetVertices(BasicShapesData::GetTriangleVertices(), verticesCount, GL_STATIC_DRAW);

		thisVAO->VBuffer = vbo;
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
		cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/FragmentShader_ColorBasedOnPosition.glsl");
	}

	RenderCommand newCommand;
	newCommand.Material = cubeMaterial;
	newCommand.VAO = thisVAO;
	newCommand.Parent = this_shared(this);
	newCommand.DrawType = EDrawCallType::DrawElements;

	RHI->AddCommand(newCommand);
}


SquareShape::SquareShape() = default;
SquareShape::~SquareShape() = default;

void SquareShape::SetupDrawCommands()
{
	const eastl::string vaoName = "squareVAO";
	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
	const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO);

	if (!existingVAO)
	{
		// TODO: Buffers creation should be delegated to the renderer
		IndexBuffer ibo = IndexBuffer{};
		int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
		ibo.SetIndices(BasicShapesData::GetSquareIndices(), indicesCount, GL_STATIC_DRAW);

		VertexBufferLayout layout = VertexBufferLayout{};
		// Vertex points
		layout.Push<float>(3);
		// Vertex Tex Coords
		layout.Push<float>(2);

		VertexBuffer vbo = VertexBuffer{ ibo, layout };
		int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
		vbo.SetVertices(BasicShapesData::GetSquareVertices(), verticesCount, GL_STATIC_DRAW);

		thisVAO->VBuffer = vbo;
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
		cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
	}

	RenderCommand newCommand;
	newCommand.Material = cubeMaterial;
	newCommand.VAO = thisVAO;
	newCommand.Parent = this_shared(this);
	newCommand.DrawType = EDrawCallType::DrawElements;

	RHI->AddCommand(newCommand);
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

void CubeShape::SetupDrawCommands()
{
	const eastl::string vaoName = "cubeVAO";
	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
	const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO);

	if (!existingVAO)
	{
		// TODO: Buffers creation should be delegated to the renderer
		IndexBuffer ibo = IndexBuffer{};
		int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
		ibo.SetIndices(BasicShapesData::GetCubeIndices(), indicesCount, GL_STATIC_DRAW);

		VertexBufferLayout layout = VertexBufferLayout{};
		// Vertex points
		layout.Push<float>(3);
		// Normals
		layout.Push<float>(3);
		// Vertex Tex Coords
		layout.Push<float>(2);

		VertexBuffer vbo = VertexBuffer{ ibo, layout };
		int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
		vbo.SetVertices(BasicShapesData::GetCubeVertices(), verticesCount, GL_STATIC_DRAW);

		thisVAO->VBuffer = vbo;
	}

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial<WithShadowMaterial>("cube_material", materialExists);

	if (!materialExists)
	{
		eastl::string texturePath = "../Data/Textures/MinecraftGrass.jpg";
		eastl::shared_ptr<OpenGLTexture> tex = eastl::make_shared<OpenGLTexture>("DiffuseMap");
		tex->Init(texturePath);
		cubeMaterial->Textures.push_back(std::move(tex));
		cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/WithNormalProjectionVertexShader.glsl", "../Data/Shaders/LightingTexFragmentShader.glsl");
	}

	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>();
	AddChild(cubeNode);

	RenderCommand newCommand;
	newCommand.Material = cubeMaterial;
	newCommand.VAO = thisVAO;
	newCommand.Parent = cubeNode;
	newCommand.DrawType = EDrawCallType::DrawArrays;

	RHI->AddCommand(newCommand);
}

Skybox::Skybox() = default;
Skybox::~Skybox() = default;

void Skybox::SetupDrawCommands()
{
	const eastl::string vaoName = "skyboxVAO";
	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
	const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO);

	if (!existingVAO)
	{
		// TODO: Buffers creation should be delegated to the renderer
		IndexBuffer ibo = IndexBuffer{};
		ibo.IndicesCount = BasicShapesData::GetSkyboxIndicesCount();
		VertexBufferLayout layout = VertexBufferLayout{};
		// Vertex points
		layout.Push<float>(3);

		VertexBuffer vbo = VertexBuffer{ ibo, layout };
		int32_t verticesCount = BasicShapesData::GetSkyboxVerticesCount();
		vbo.SetVertices(BasicShapesData::GetSkyboxVertices(), verticesCount, GL_STATIC_DRAW);

		thisVAO->VBuffer = vbo;
	}

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> thisMaterial = matManager.GetOrAddMaterial<SkyboxMaterial>("skybox_material", materialExists);

	if (!materialExists)
	{
		eastl::vector<eastl::string> textures
		{
			"../Data/Textures/skybox/right.jpg",
			"../Data/Textures/skybox/left.jpg",
			"../Data/Textures/skybox/top.jpg",
			"../Data/Textures/skybox/bottom.jpg",
			"../Data/Textures/skybox/front.jpg",
			"../Data/Textures/skybox/back.jpg",
		};

		eastl::shared_ptr<OpenGLCubeMap> tex = eastl::make_shared<OpenGLCubeMap>("CubeMap");
		tex->Init(textures);
		thisMaterial->Textures.push_back(std::move(tex));
		thisMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/SkyboxVertexShader.glsl", "../Data/Shaders/SkyboxFragmentShader.glsl");
	}

	eastl::shared_ptr<MeshNode> thisNode = eastl::make_shared<MeshNode>();
	AddChild(thisNode);

	RenderCommand newCommand;
	newCommand.Material = thisMaterial;
	newCommand.VAO = thisVAO;
	newCommand.Parent = thisNode;
	newCommand.DrawType = EDrawCallType::DrawArrays;

	RHI->SetSkyboxCommand(newCommand);
}

LightSource::LightSource() = default;
LightSource::~LightSource() = default;

void LightSource::SetupDrawCommands()
{
	const eastl::string vaoName = "lightSourceVAO";
	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
	const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO);

	if (!existingVAO)
	{
		// TODO: Buffers creation should be delegated to the renderer
		IndexBuffer ibo = IndexBuffer{};
		int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
		ibo.SetIndices(BasicShapesData::GetCubeIndices(), indicesCount, GL_STATIC_DRAW);

		VertexBufferLayout layout = VertexBufferLayout{};
		// Vertex points
		layout.Push<float>(3);
		// Normals
		layout.Push<float>(3);
		// Vertex Tex Coords
		layout.Push<float>(2);

		VertexBuffer vbo = VertexBuffer{ ibo, layout };
		int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
		vbo.SetVertices(BasicShapesData::GetCubeVertices(), verticesCount, GL_STATIC_DRAW);

		thisVAO->VBuffer = vbo;
	}

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial<RenderMaterial>("light_source_material", materialExists);

	if (!materialExists)
	{
		cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/WithNormalProjectionVertexShader.glsl", "../Data/Shaders/LightSourceFragmentShader.glsl");
	}

	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>();
	AddChild(cubeNode);

	RenderCommand newCommand;
	newCommand.Material = cubeMaterial;
	newCommand.VAO = thisVAO;
	newCommand.Parent = cubeNode;
	newCommand.DrawType = EDrawCallType::DrawArrays;

	RHI->AddCommand(newCommand);
}
