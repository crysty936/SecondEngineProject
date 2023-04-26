#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapes.h"
#include "Controller/ControllerBase.h"
#include "glm/common.hpp"
#include <stdlib.h>
#include "Renderer/Model/3D/Assimp/AssimpModel3D.h"
#include "Core/EntityHelper.h"
#include "Renderer/RHI/Resources/MeshDataContainer.h"
#include "Renderer/ForwardRenderer.h"
#include "Renderer/RHI/Resources/VertexInputLayout.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"
#include "Renderer/RHI/RHI.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Utils/InlineVector.h"
#include "Renderer/Drawable/Materials/RenderMaterial_Parallax.h"
#include "Renderer/Drawable/Materials/RenderMaterial_Billboard.h"
#include "Renderer/DrawDebugHelpers.h"

TestGameMode GGameMode = {};

TestGameMode::TestGameMode() = default;
TestGameMode::~TestGameMode() = default;

class InstancedAssimpModelTest : public AssimpModel3D
{
public:
	InstancedAssimpModelTest(const eastl::string& inPath, const eastl::string& inName)
		: AssimpModel3D(inPath, inName) {}
	virtual ~InstancedAssimpModelTest() = default;

protected:
	virtual eastl::shared_ptr<RHIShader> CreateShaders(const class VertexInputLayout& inLayout) const override
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "VS_Pos-UV-Normal_Instanced", EShaderType::Vertex },
		{ "PS_BasicTex", EShaderType::Fragment } };

		return RHI::Instance->CreateShaderFromPath(shaders, inLayout);
	}


	virtual void AddAdditionalBuffers(eastl::shared_ptr<MeshDataContainer>& inDataContainer) const override
	{
		// Instance data
		glm::mat4* instanceOffsets = new glm::mat4[InstancesCount * InstancesCount];

		for (int32_t i = 0; i < InstancesCount; ++i)
		{
			for (int32_t j = 0; j < InstancesCount; ++j)
			{
				glm::mat4& instanceMat = instanceOffsets[i * InstancesCount + j];

				instanceMat = glm::identity<glm::mat4>();

				instanceMat = glm::translate(instanceMat, glm::vec3(4.f * i, 4.f * j, 0.f));
			}
		}

		VertexInputLayout vertexInstanceLayout;
		vertexInstanceLayout.Push<float>(4, VertexInputType::InstanceData, EAttribDivisor::PerInstance);
		vertexInstanceLayout.Push<float>(4, VertexInputType::InstanceData, EAttribDivisor::PerInstance);
		vertexInstanceLayout.Push<float>(4, VertexInputType::InstanceData, EAttribDivisor::PerInstance);
		vertexInstanceLayout.Push<float>(4, VertexInputType::InstanceData, EAttribDivisor::PerInstance);

		vertexInstanceLayout.AttribsOffset = 3;

		const eastl::shared_ptr<RHIVertexBuffer> instanceBuffer = RHI::Instance->CreateVertexBuffer(vertexInstanceLayout, &instanceOffsets[0], sizeof(glm::mat4) * InstancesCount * InstancesCount);
		inDataContainer->AdditionalBuffers.push_back(instanceBuffer);
	}


	virtual RenderCommand CreateRenderCommand(eastl::shared_ptr<RenderMaterial>& inMaterial, eastl::shared_ptr<MeshNode>& inParent, eastl::shared_ptr<MeshDataContainer>& inDataContainer) override
	{
		RenderCommand newCommand;
		newCommand.Material = inMaterial;
		newCommand.Parent = inParent;
		newCommand.DataContainer = inDataContainer;
		newCommand.DrawType = EDrawType::DrawInstanced;
		newCommand.InstancesCount = InstancesCount * InstancesCount;
		//newCommand.DrawPasses = static_cast<EDrawMode::Type>(EDrawMode::Default | EDrawMode::NORMAL_VISUALIZE);

		return newCommand;
	}

	const int32_t InstancesCount = 10;
};

// Instanced textured cube
class InstancedCubeTest : public Model3D
{
public:
	InstancedCubeTest(const eastl::string& inName)
		: Model3D(inName)
	{

	}
	virtual ~InstancedCubeTest() = default;

	virtual void CreateProxy() override
	{
 		const eastl::string RenderDataContainerID = "instancedCubeVAO";
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
			eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Instance->CreateIndexBuffer(BasicShapesData::GetCubeIndices(), indicesCount);


			int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
			const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Instance->CreateVertexBuffer(inputLayout, BasicShapesData::GetCubeVertices(), verticesCount, ib);

			dataContainer->VBuffer = vb;
		}

		// Instance data
		constexpr int32_t instancesCount = 10;
		glm::mat4* instanceOffsets = new glm::mat4[instancesCount * instancesCount];

		for (int32_t i = 0; i < instancesCount; ++i)
		{
			for (int32_t j = 0; j < instancesCount; ++j)
			{
				glm::mat4& instanceMat = instanceOffsets[i * instancesCount + j];

				instanceMat = glm::identity<glm::mat4>();
				instanceMat = glm::translate(instanceMat, glm::vec3(glm::sin(0.5 * j) * 40, 10.f, glm::cos(i * 0.5 * j) * 40));
			}
		}

		VertexInputLayout vertexInstanceLayout;
		vertexInstanceLayout.Push<float>(4, VertexInputType::InstanceData, EAttribDivisor::PerInstance);
		vertexInstanceLayout.Push<float>(4, VertexInputType::InstanceData, EAttribDivisor::PerInstance);
		vertexInstanceLayout.Push<float>(4, VertexInputType::InstanceData, EAttribDivisor::PerInstance);
		vertexInstanceLayout.Push<float>(4, VertexInputType::InstanceData, EAttribDivisor::PerInstance);

		vertexInstanceLayout.AttribsOffset = 3;
		
		const eastl::shared_ptr<RHIVertexBuffer> instanceBuffer = RHI::Instance->CreateVertexBuffer(vertexInstanceLayout, &instanceOffsets[0], sizeof(glm::mat4) * instancesCount * instancesCount);
		dataContainer->AdditionalBuffers.push_back(instanceBuffer);

  		MaterialsManager& matManager = MaterialsManager::Get();
  		bool materialExists = false;
  		eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial("instanced_cube_material", materialExists);
  
  		if (!materialExists)
  		{
  			eastl::shared_ptr<RHITexture2D> tex = RHI::Instance->CreateTexture2D("../Data/Textures/MinecraftGrass.jpg");
  			material->OwnedTextures.push_back(tex);
  
  			eastl::vector<ShaderSourceInput> shaders = {
  			{ "Pos-UV-Normal_BasicTex_Instanced/VS_Pos-UV-Normal_Instanced", EShaderType::Vertex },
  			{ "Pos-UV-Normal_BasicTex_Instanced/PS_BasicTex", EShaderType::Fragment } };
  
  			material->Shader = RHI::Instance->CreateShaderFromPath(shaders, inputLayout);
  		}
  
  		eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>("Cube Mesh");
  		AddChild(cubeNode);
  
  		RenderCommand newCommand;
  		newCommand.Material = material;
  		newCommand.DataContainer = dataContainer;
  		newCommand.Parent = cubeNode;
  		newCommand.DrawType = EDrawType::DrawInstanced;
		newCommand.InstancesCount = instancesCount * instancesCount;
  
  		ForwardRenderer::Get().AddCommand(newCommand);
	}
};

class ParallaxQuad : public Model3D
{
public:
	ParallaxQuad(const eastl::string& inName)
		: Model3D(inName) {}
	virtual ~ParallaxQuad() = default;

	virtual void CreateProxy() override
	{
		const eastl::string RenderDataContainerID = "ParallaxQuadContainer";
		eastl::shared_ptr<MeshDataContainer> dataContainer;

		const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);
		VertexInputLayout inputLayout;

		// Vertex points
		inputLayout.Push<float>(3, VertexInputType::Position);
		// Vertex Normal
		inputLayout.Push<float>(3, VertexInputType::Normal);
		// Vertex Tex Coords
		inputLayout.Push<float>(2, VertexInputType::TexCoords);
 		// Tangent
 		inputLayout.Push<float>(3, VertexInputType::Tangent);
 		// Bitangent
 		inputLayout.Push<float>(3, VertexInputType::Bitangent);

		if (!existingContainer)
		{
			int32_t indicesCount = BasicShapesData::GetTBNQuadIndicesCount();
			eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Instance->CreateIndexBuffer(BasicShapesData::GetTBNQuadIndices(), indicesCount);


			int32_t verticesCount = BasicShapesData::GetTBNQuadVerticesCount();
			const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Instance->CreateVertexBuffer(inputLayout, BasicShapesData::GetTBNQuadVertices(), verticesCount, ib);

			dataContainer->VBuffer = vb;
		}

		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_Parallax>("parallax_quad_material", materialExists);

		if (!materialExists)
		{
			eastl::shared_ptr<RHITexture2D> diffMap = RHI::Instance->CreateTexture2D("../Data/Textures/Parallax/bricks.jpg");
			eastl::shared_ptr<RHITexture2D> normalMap = RHI::Instance->CreateTexture2D("../Data/Textures/Parallax/bricks_normal.jpg");
			eastl::shared_ptr<RHITexture2D> heightMap = RHI::Instance->CreateTexture2D("../Data/Textures/Parallax/bricks_disp.jpg");

			material->OwnedTextures.push_back(diffMap);
			material->OwnedTextures.push_back(normalMap);
			material->OwnedTextures.push_back(heightMap);

			eastl::vector<ShaderSourceInput> shaders = {
			{ "Parallax/VS_Pos-UV-Normal-Tangent-Bitangent_Model_WorldPosition", EShaderType::Vertex },
			{ "Parallax/PS_ParallaxTest", EShaderType::Fragment } };

			material->Shader = RHI::Instance->CreateShaderFromPath(shaders, inputLayout);
		}

		eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>("Mesh");
		AddChild(cubeNode);

		RenderCommand newCommand;
		newCommand.Material = material;
		newCommand.DataContainer = dataContainer;
		newCommand.Parent = cubeNode;
		newCommand.DrawType = EDrawType::DrawElements;

		ForwardRenderer::Get().AddCommand(newCommand);
	}
};

class BillboardQuad : public Model3D
{
public:
	BillboardQuad() = default;
	virtual ~BillboardQuad() = default;

	virtual void CreateProxy() override
	{
		const eastl::string RenderDataContainerID = "BillboardQuadContainer";
		eastl::shared_ptr<MeshDataContainer> dataContainer;

		const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(RenderDataContainerID, dataContainer);
		VertexInputLayout inputLayout;

		// Vertex points
		inputLayout.Push<float>(3, VertexInputType::Position);
		// Vertex Tex Coords
		inputLayout.Push<float>(2, VertexInputType::TexCoords);

		if (!existingContainer)
		{
			int32_t indicesCount = BasicShapesData::GetQuadIndicesCount();
			eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Instance->CreateIndexBuffer(BasicShapesData::GetQuadIndices(), indicesCount);


			int32_t verticesCount = BasicShapesData::GetQuadVerticesCount();
			const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Instance->CreateVertexBuffer(inputLayout, BasicShapesData::GetQuadVertices(), verticesCount, ib);

			dataContainer->VBuffer = vb;
		}

		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial<RenderMaterial_Billboard>("billboard_quad_material", materialExists);

		if (!materialExists)
		{
			eastl::shared_ptr<RHITexture2D> diffMap = RHI::Instance->CreateTexture2D("../Data/Textures/MinecraftGrass.jpg");

			material->OwnedTextures.push_back(diffMap);

			eastl::vector<ShaderSourceInput> shaders = {
			{ "Pos-UV_BasicTex_Billboard/VS_Pos-UV_Billboard", EShaderType::Vertex },
			{ "Pos-UV_BasicTex_Billboard/PS_BasicTex", EShaderType::Fragment } };

			material->Shader = RHI::Instance->CreateShaderFromPath(shaders, inputLayout);
		}

		eastl::shared_ptr<MeshNode> billboardNode = eastl::make_shared<MeshNode>("Mesh");
		AddChild(billboardNode);

		RenderCommand newCommand;
		newCommand.Material = material;
		newCommand.DataContainer = dataContainer;
		newCommand.Parent = billboardNode;
		newCommand.DrawType = EDrawType::DrawElements;

		ForwardRenderer::Get().AddCommand(newCommand);
	}
};


void TestGameMode::Init()
{
	//Controller = eastl::make_unique<GameController>(); // TODO: Do this on a pre-game init or something

	// TODO: // This should normally be done by the scene loader so game mode only gets accessed when game starts
	// 
	// Scene setup 
	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	GameCamera = currentScene.GetCurrentCamera();
	//GameCamera->SetMovementDelegates(*Controller);

	// Push camera back a bit
	if (TransformObjPtr parentShared = GameCamera->GetParent().lock())
	{
		// Move the camera parent
		parentShared->Move(glm::vec3(0.f, 0.f, 10.f));
	}

 	{
// 		// Ground
//         	 	eastl::shared_ptr<CubeShape> centerObj = BasicShapesHelpers::CreateCubeObject();
//         		centerObj->Move(glm::vec3(0.f, -2.f, 0.f));
//         		centerObj->SetScale(glm::vec3(100.f, 0.5f, 100.f));
 	}
	{
		//eastl::shared_ptr<CubeShape> centerObj = BasicShapesHelpers::CreateCubeObject();

		//eastl::shared_ptr<SquareShape> SquareTestObj = BasicShapesHelpers::CreateSquareObject();

		//eastl::shared_ptr<CubeShape> lightObj = BasicShapesHelpers::CreateCubeObject();
		//lightObj->SetRelativeLocation(glm::vec3(10.f, 20.f, 15.f));
	}
// 	{
//  		eastl::shared_ptr<CubeShape> centerObj = BasicShapesHelpers::CreateCubeObject();
//  		centerObj->Move(glm::vec3(0.f, 0.f, 5.f));
// 	}
 	{
// 		for (int32_t j = 0; j < 100; ++j)
// 		{
// 			for (int32_t i = 0; i < 100; ++i)
// 			{
//  				eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
//  				centerObj->Move(glm::vec3(j * 3.f, 5.f, i * 3.f));
// 				//centerObj->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
// 			}
// 		}
 	}

	// Light
// 	eastl::shared_ptr<LightSource> lightObj = EntityHelper::CreateObject<LightSource>();
// 	lightObj->SetRelativeLocation(glm::vec3(1000, 20000.0f, -1000.f));
// 	lightObj->SetScale(glm::vec3(100.f, 100.f, 100.f));

  	AssimpModel = eastl::shared_ptr<AssimpModel3D>(EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/Shiba/scene.gltf", "Shiba"));
  	AssimpModel->SetScale(glm::vec3(10.f, 10.f, 10.f));
  	AssimpModel->Move(glm::vec3(0.f, 10.f, 0.f));

	FloorModel = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/Floor/scene.gltf", "Floor");
	FloorModel->Move(glm::vec3(0.f, -2.f, 0.f));
	//floorModel->SetScale(glm::vec3(10.f, 1.f, 10.f));

	//TransformObjPtr concreteFloor = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/ConcreteFloor/Source/plane.fbx");
// 	TransformObjPtr concreteFloor = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/ConcreteFloorGLTF/scene.gltf");
// 	concreteFloor->Move(glm::vec3(5.f, 0.f, 0.f));


	//Quad = EntityHelper::CreateObject<ParallaxQuad>("Parallax Quad");
	//Quad->Rotate(90.f, glm::vec3(-1.f, 0.f, 0.f));

	//eastl::shared_ptr<AssimpModel3D> sponzaModel = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/Sponza/scene.gltf");

//  	eastl::shared_ptr<AssimpModel3D> model = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/Backpack/scene.gltf");
//  	model->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
//  	model->Move(glm::vec3(0.f, 3.f, 0.f));


	eastl::shared_ptr<InstancedCubeTest> instancedObj = EntityHelper::CreateObject<InstancedCubeTest>("Instanced Cubes");
	//TransformObjPtr concreteFloor = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/ConcreteFloorGLTF/scene.gltf");
	//Billboard = EntityHelper::CreateObject<BillboardQuad>();

  	//eastl::shared_ptr<InstancedAssimpModelTest> instancedObj = EntityHelper::CreateObject<InstancedAssimpModelTest>("../Data/Models/Backpack/scene.gltf");
  	//instancedObj->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
  	//instancedObj->Move(glm::vec3(0.f, 3.f, 0.f));



//  	{
//  		eastl::shared_ptr<MirrorQuad> test = ObjectCreation::NewObject<MirrorQuad>();
//  
//  		test->Move(glm::vec3(0.0f, 0.0f, -10.f));
//  	}

//    	eastl::shared_ptr<AssimpModel3D> model = ObjectCreation::NewObject<AssimpModel3D>("../Data/Models/Shiba/scene.gltf");
//    	model->Move(glm::vec3(0.f, 0.f, -5.f));
   	//model->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));

	//eastl::shared_ptr<Skybox> skybox = ObjectCreation::NewObject<Skybox>();
// 


//   	{
// 		DirLight = EntityHelper::CreateObject<LightSource>("Directional Light");
// 		DirLight->Data.Type = ELightType::Directional;
// 		DirLight->SetRelativeLocation({ -2.0f, 20.0f, -1.0f });
// 		DirLight->SetRotationDegrees(glm::vec3(80.f, 0.f, 0.f));
//   	}

	constexpr float linear = 0.007f;
	constexpr float quadratic = 0.0002f;

	{
		eastl::shared_ptr<LightSource> PointLight = EntityHelper::CreateObject<LightSource>("Point Light 1");
		PointLight->Data.Type = ELightType::Point;
		PointLight->Data.TypeData.PointData.Linear = linear;
		PointLight->Data.TypeData.PointData.Quadratic = quadratic;
		PointLight->Data.TypeData.PointData.Color = glm::vec3(0.f, 0.5f, 0.f);

		PointLight->SetRelativeLocation({ -10.f, 1.0f, 35.f });
	}

	{
		eastl::shared_ptr<LightSource> PointLight = EntityHelper::CreateObject<LightSource>("Point Light 2");
		PointLight->Data.Type = ELightType::Point;
		PointLight->Data.TypeData.PointData.Linear = linear;
		PointLight->Data.TypeData.PointData.Quadratic = quadratic;
		PointLight->Data.TypeData.PointData.Color = glm::vec3(0.f, 0.f, 0.2f);

		PointLight->SetRelativeLocation({ -65.f, 1.0f, 0.f });
	}

	{
		eastl::shared_ptr<LightSource> PointLight = EntityHelper::CreateObject<LightSource>("Point Light 3");
		PointLight->Data.Type = ELightType::Point;
		PointLight->Data.TypeData.PointData.Linear = linear;
		PointLight->Data.TypeData.PointData.Quadratic = quadratic;
		PointLight->Data.TypeData.PointData.Color = glm::vec3(100.f, 100.f, 100.f);

		PointLight->SetRelativeLocation({ 22.f, 1.0f, -18.f });
	}
}

void TestGameMode::Tick(float inDeltaT)
{
	//Controller->ExecuteCallbacks();

	//AssimpModel->Rotate(0.1f, glm::vec3(1.f, 0.f, 0.f));

	//FloorModel->Rotate(0.1f, glm::vec3(1.f, 0.f, 0.f));

	//Quad->Rotate(0.1f, glm::vec3(-1.f, 0.f, 0.f));

	glm::vec3 forward = glm::vec3(0.f, 0.f, 1.f);
	glm::vec3 right = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);

	const eastl::shared_ptr<LightSource>& usedLight = DirLight;

	if (usedLight)
	{
 		const glm::quat& rot = usedLight->GetAbsoluteTransform().Rotation;
 
 		// order of operations matters, considered like a matrix
 		forward = glm::normalize(rot * forward);
 		right = glm::normalize(rot * right);
 		up = glm::normalize(rot * up);
 
 		const glm::vec3 start = usedLight->GetAbsoluteTransform().Translation;
 
 		DrawDebugHelpers::DrawDebugLine(start, start + forward * 20.f, glm::vec3(0.f, 0.f, 1.f));
 		DrawDebugHelpers::DrawDebugLine(start, start + right * 20.f, glm::vec3(1.f, 0.f, 0.f));
 		DrawDebugHelpers::DrawDebugLine(start, start + up * 20.f, glm::vec3(0.f, 1.f, 0.f));
	}




}
