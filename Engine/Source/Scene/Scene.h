#pragma once
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"
#include "Entity/Entity.h"

/**
 * Scene class precursor
 * Scene should contain all objects present in that scene as gameobjects.
 * It has a strong reference to them and each time an object get's destroyed, it get's removed from the scene's list
 * 
 */

class Scene
{
public:
	Scene();
	~Scene();

	void TickObjects(float inDeltaT);
	void InitObjects();
	void AddEntity(EntityPtr inEntity);

private:
	void RecursivelyTickObjects(float inDeltaT, eastl::vector<eastl::shared_ptr<class Entity>>& inObjects);
	void RecursivelyInitObjects(eastl::vector<eastl::shared_ptr<class Entity>>& inObjects);

public:
	eastl::vector<eastl::shared_ptr<class Entity>> Entities;
	eastl::shared_ptr<class Camera> CurrentCamera;
};