#include "SceneManager.h"
#include "Core/EngineUtils.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"

SceneManager* SceneManager::Instance = nullptr;

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::Init()
{
	Instance = new SceneManager{};
}

void SceneManager::LoadScene()
{
	// TODO Should be implemented to read the scene from a serialized input

	Instance->CurrentScene = eastl::make_unique<class Scene>();
	//Instance->CurrentScene->SceneObjects.push_back(eastl::make_shared<Camera>());







	Instance->CurrentScene->SceneObjects.push_back(eastl::make_shared<Camera>());



}

void SceneManager::Terminate()
{
	ASSERT(Instance);

	delete Instance;
}

