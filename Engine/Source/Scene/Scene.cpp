#include "Scene.h"
#include "Core/IGameObject.h"

Scene::Scene() = default;
Scene::~Scene() = default;

void Scene::TickObjects(float inDeltaT)
{
	RecursivelyTickObjects(inDeltaT, SceneObjects);
}

void Scene::InitObjects()
{
	RecursivelyInitObjects(SceneObjects);
}

void Scene::RecursivelyTickObjects(float inDeltaT, eastl::vector<eastl::shared_ptr<IGameObject>>& inObjects)
{
	for (eastl::shared_ptr<IGameObject>& obj : inObjects)
	{
		RecursivelyTickObjects(inDeltaT, obj->Children);

		obj->Tick(inDeltaT);
	}
}

void Scene::RecursivelyInitObjects(eastl::vector<eastl::shared_ptr<IGameObject>>& inObjects)
{
	for (eastl::shared_ptr<IGameObject>& obj : inObjects)
	{
		RecursivelyInitObjects(obj->Children);

		obj->Init();
	}
}