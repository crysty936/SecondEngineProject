#include "FlappyGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/OpenGL/OpenGLRenderableObject.h"
#include "Renderer/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"

const float DecelerationSpeed = 5.f;

FlappyGameMode::FlappyGameMode()
	: GameModeBase(), GameCamera{ nullptr }, BirdObject{ nullptr }, GameController{}
{
}

FlappyGameMode::~FlappyGameMode() = default;

void FlappyGameMode::Init()
{
	GameController = eastl::make_unique<class Controller>();
	
	// Scene setup
	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	GameCamera = currentScene.CurrentCamera;

	GameCamera->Model.Translation.z = 15.f;
	GameCamera->Model.Rotation.z = -1.f;

	BirdObject = BasicShapes::CreateSquareObject();
	
	currentScene.SceneObjects.push_back(BirdObject);
}


void FlappyGameMode::Tick(float inDelta)
{
	float decelerationOffset = DecelerationSpeed * inDelta;

	BirdObject->Model.Translation.x -= decelerationOffset;
	BirdObject->Model.Translation.y -= decelerationOffset;






}

