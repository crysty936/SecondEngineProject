#include "Scene.h"
#include "Entity/Entity.h"

Scene::Scene() = default;
Scene::~Scene() = default;

void Scene::TickObjects(float inDeltaT)
{
	RecursivelyTickObjects(inDeltaT, Entities);
}

void Scene::InitObjects()
{
	RecursivelyInitObjects(Entities);
}

void Scene::AddEntity(EntityPtr inEntity)
{
	Entities.push_back(inEntity);
}

void Scene::RecursivelyTickObjects(float inDeltaT, eastl::vector<eastl::shared_ptr<Entity>>& inObjects)
{
	for (eastl::shared_ptr<Entity>& obj : inObjects)
	{
		RecursivelyTickObjects(inDeltaT, obj->Children);

		obj->Tick(inDeltaT);
	}
}

void Scene::RecursivelyInitObjects(eastl::vector<eastl::shared_ptr<Entity>>& inObjects)
{
	for (eastl::shared_ptr<Entity>& obj : inObjects)
	{
		obj->Init();

		RecursivelyInitObjects(obj->Children);
	}
}