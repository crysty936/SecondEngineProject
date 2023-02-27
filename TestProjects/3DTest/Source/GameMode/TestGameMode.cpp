#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"
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

TestGameMode GGameMode = {};

TestGameMode::TestGameMode() = default;
TestGameMode::~TestGameMode() = default;


class InstancedAssimpModelTest : public AssimpModel3D
{
public:
	InstancedAssimpModelTest(const eastl::string& inPath)
		: AssimpModel3D(inPath) {}
	virtual ~InstancedAssimpModelTest() = default;

protected:
	virtual eastl::shared_ptr<RHIShader> CreateShaders(const class VertexInputLayout& inLayout) const override
	{
		eastl::vector<ShaderSourceInput> shaders = {
		{ "ModelWorldPosition_VS_Pos-UV-Normal_ManuallyWritten-Instanced", EShaderType::Vertex },
		{ "BasicTex_PS", EShaderType::Fragment } };

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
		newCommand.DrawType = EDrawCallType::DrawInstanced;
		newCommand.InstancesCount = InstancesCount * InstancesCount;
		//newCommand.DrawPasses = static_cast<EDrawMode::Type>(EDrawMode::Default | EDrawMode::NORMAL_VISUALIZE);

		return newCommand;
	}

	const int32_t InstancesCount = 10;
};

// Instanced textured cube
class InstancedCubeTest : public CubeShape
{
public:
	InstancedCubeTest() = default;
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
  			{ "ModelWorldPosition_VS_Pos-UV-Normal_ManuallyWritten-Instanced", EShaderType::Vertex },
  			{ "BasicTex_PS", EShaderType::Fragment } };
  
  			material->Shader = RHI::Instance->CreateShaderFromPath(shaders, inputLayout);
  		}
  
  		eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>();
  		AddChild(cubeNode);
  
  		RenderCommand newCommand;
  		newCommand.Material = material;
  		newCommand.DataContainer = dataContainer;
  		newCommand.Parent = cubeNode;
  		newCommand.DrawType = EDrawCallType::DrawInstanced;
		newCommand.InstancesCount = instancesCount * instancesCount;
  
  		ForwardRenderer::Get().AddCommand(newCommand);
	}
};

void TestGameMode::Init()
{
	GameController = eastl::make_unique<Controller>();

	// Debug
	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraLeft);
		EInputKey createFloorKey = EInputKey::A;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraRight);
		EInputKey createFloorKey = EInputKey::D;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };

		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraUp);
		EInputKey createFloorKey = EInputKey::W;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraDown);
		EInputKey createFloorKey = EInputKey::S;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };

		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::OnChangeDrawMode);
		EInputKey createFloorKey = EInputKey::F;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, true };

		GameController->AddListener(createFloorAction);
	}
	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::DebugProjections);
		EInputKey createFloorKey = EInputKey::G;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, true };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::DebugCascadesVisualize);
		EInputKey createFloorKey = EInputKey::T;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, true };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::DebugCursorMode);
		EInputKey createFloorKey = EInputKey::Q;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, true };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::BoostCameraSpeed);
		EInputKey createFloorKey = EInputKey::LeftShift;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, true };
		GameController->AddListener(createFloorAction);
	}



	// Debug

	// Scene setup
	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	GameCamera = currentScene.CurrentCamera;
	GameCamera->SetMovementDelegates(*GameController);

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
	eastl::shared_ptr<LightSource> lightObj = EntityHelper::CreateObject<LightSource>();
	lightObj->SetRelativeLocation(glm::vec3(-5.0f, 20.0f, -0.2f));

 	eastl::shared_ptr<AssimpModel3D> shibaModel = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/Shiba/scene.gltf");
 	shibaModel->SetScale(glm::vec3(10.f, 10.f, 10.f));
 	shibaModel->Move(glm::vec3(0.f, 10.f, 0.f));

 	eastl::shared_ptr<AssimpModel3D> floorModel = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/Floor/scene.gltf");
	floorModel->Move(glm::vec3(0.f, -2.f, 0.f));
	//floorModel->SetScale(glm::vec3(10.f, 1.f, 10.f));


	//eastl::shared_ptr<AssimpModel3D> sponzaModel = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/Sponza/scene.gltf");

//  	eastl::shared_ptr<AssimpModel3D> model = EntityHelper::CreateObject<AssimpModel3D>("../Data/Models/Backpack/scene.gltf");
//  	model->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
//  	model->Move(glm::vec3(0.f, 3.f, 0.f));


	eastl::shared_ptr<InstancedCubeTest> instancedObj = EntityHelper::CreateObject<InstancedCubeTest>();


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
//  	{
//  		eastl::shared_ptr<LightSource> lightSource = ObjectCreation::NewObject<LightSource>();
//  		//lightSource->Move(glm::vec3(0.f, 15.f, 0.f));
//  		lightSource->SetRelativeLocation({ -2.0f, 20.0f, -1.0f });
// 		lightSource->Rotate(20.f, glm::vec3(0.f, 1.f, 0.f));
//  	}

}

float CameraSpeed = 0.4f;

void TestGameMode::Tick(float inDeltaT)
{
	GameController->ExecuteCallbacks();
}

void TestGameMode::MoveCameraLeft()
{
	GameCamera->Move(MovementDirection::Left, CameraSpeed);
	//GameCamera->GetRelativeTransform().Rotate(5.f, glm::vec3(0.f, 1.f, 0.f));
}

void TestGameMode::MoveCameraRight()
{
	GameCamera->Move(MovementDirection::Right, CameraSpeed);
	//GameCamera->GetRelativeTransform().Rotate(-5.f, glm::vec3(0.f, 1.f, 0.f));
}

void TestGameMode::MoveCameraUp()
{
	GameCamera->Move(MovementDirection::Forward, CameraSpeed);
}

void TestGameMode::MoveCameraDown()
{
	GameCamera->Move(MovementDirection::Back, CameraSpeed);
}

void TestGameMode::OnChangeDrawMode()
{
	static bool drawMode = false;

	//RHI->SetDrawMode(drawMode ? EDrawMode::NORMAL : EDrawMode::DEPTH);

	drawMode = !drawMode;
}

void TestGameMode::DebugProjections()
{
	ForwardRenderer::Get().UpdateShadowMatrices = !ForwardRenderer::Get().UpdateShadowMatrices;
}

void TestGameMode::DebugCascadesVisualize()
{
	ForwardRenderer::Get().bCascadeVisualizeMode = !ForwardRenderer::Get().bCascadeVisualizeMode;
}

void TestGameMode::DebugCursorMode()
{
	static bool test = true;

	const ECursorMode mode = test ? ECursorMode::Enabled : ECursorMode::Disabled;
	InputSystem::Get().SetCursorMode(mode);

	test = !test;
}


void TestGameMode::BoostCameraSpeed()
{
	static bool boostOn = false;

	if (boostOn)
	{
		CameraSpeed = 0.4f;
	}
	else
	{
		CameraSpeed = 10.f;
	}

	boostOn = !boostOn;

}



