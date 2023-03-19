#include "Scene.h"
#include "Entity/Entity.h"
#include "imgui.h"

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

void Scene::DisplayObjects()
{
	ImGui::Begin("Scene");

	RecursivelyDisplayObjects(Objects, true);

	ImGui::End();
}



void Scene::RecursivelyTickObjects(float inDeltaT, eastl::vector<TransformObjPtr>& inObjects)
{
	for (TransformObjPtr& obj : inObjects)
	{
		RecursivelyTickObjects(inDeltaT, obj->Children);

		// TODO: Place different objects in different lists, waste of cycles to do dynamic cast on all each frame
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

void Scene::RecursivelyDisplayObjects(eastl::vector<TransformObjPtr>& inObjects, const bool inDisplayNode)
{
	for (TransformObjPtr& obj : inObjects)
	{
		bool displayChildNodes = false;
		if (inDisplayNode)
		{
			displayChildNodes = ImGui::TreeNode(obj->Name.c_str());

			if (displayChildNodes)
			{
				if (ImGui::InputFloat3("Position", &obj->Location.x))
				{
					obj->MakeTransfDirty();
				}
			}
		}

		RecursivelyDisplayObjects(obj->Children, displayChildNodes);

		if (displayChildNodes)
		{
			ImGui::TreePop();
		}
	}
}
