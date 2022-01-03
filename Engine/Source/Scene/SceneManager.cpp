#include "SceneManager.h"
#include "Core/EngineUtils.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Renderer/RenderableModel.h"
#include "Renderer/ShapesUtils/BasicShapes.h"

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
	Instance->CurrentScene->SceneObjects.push_back(eastl::make_shared<Camera>());

	eastl::shared_ptr<RenderableModel> obj = BasicShapes::GetSquareRenderable();
 	Instance->CurrentScene->SceneObjects.push_back(obj);
}

void SceneManager::Terminate()
{
	ASSERT(Instance);

	delete Instance;
}

