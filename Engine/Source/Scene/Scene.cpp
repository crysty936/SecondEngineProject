#include "Scene.h"
#include "Entity/Entity.h"

Scene::Scene() = default;
Scene::~Scene() = default;

void Scene::TickObjects(float inDeltaT)
{
	RecursivelyTickObjects(inDeltaT, Objects);
}

void Scene::InitObjects()
{
	RecursivelyInitObjects(Objects);
}

void Scene::AddObject(TransformObjPtr inObj)
{
	Objects.push_back(inObj);
}

void Scene::RecursivelyTickObjects(float inDeltaT, eastl::vector<TransformObjPtr>& inObjects)
{
	for (TransformObjPtr& obj : inObjects)
	{
		RecursivelyTickObjects(inDeltaT, obj->Children);

		if (Entity* entt = dynamic_cast<Entity*>(obj.get()))
		{
			entt->Tick(inDeltaT);
		}
	}
}

void Scene::RecursivelyInitObjects(eastl::vector<TransformObjPtr>& inObjects)
{
	for (TransformObjPtr& obj : inObjects)
	{
		if (Entity* entt = dynamic_cast<Entity*>(obj.get()))
		{
			entt->Init();
		}

		RecursivelyInitObjects(obj->Children);
	}
}