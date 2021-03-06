#include "SceneManager.h"
#include "Core/EngineUtils.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapes.h"

SceneManager* SceneManager::Instance = nullptr;

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::Init()
{
	Instance = new SceneManager{};
}

void SceneManager::LoadScene()
{
	// TODO Should be implemented to read the scene from a serialized output

	Instance->CurrentScene = eastl::make_unique<class Scene>();
	Instance->CurrentScene->CurrentCamera = eastl::make_shared<Camera>();

	// !Trying to copy the gameplay engine tactic to see if it works
	EntityPtr CameraParent = eastl::make_shared<Entity>();
	CameraParent->AddChild(Instance->CurrentScene->CurrentCamera);
	Instance->CurrentScene->AddObject(CameraParent);
}

void SceneManager::Terminate()
{
	ASSERT(Instance);

	delete Instance;
}

