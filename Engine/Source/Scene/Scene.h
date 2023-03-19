#pragma once
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"
#include "Entity/TransformObject.h"

/**
 * Scene class precursor
 * Scene should contain all objects present in that scene as gameobjects.
 * It has a strong reference to them and each time an object get's destroyed, it get's removed from the scene's list
 * 
 */

// TODO: Tick and Init can be made using delegates

class Scene
{
public:
	Scene();
	~Scene();

	void TickObjects(float inDeltaT);
	void InitObjects();
	void AddObject(TransformObjPtr inObj);
	

	void DisplayObjects();

private:
	void RecursivelyTickObjects(float inDeltaT, eastl::vector<TransformObjPtr>& inObjects);
	void RecursivelyInitObjects(eastl::vector<TransformObjPtr>& inObjects);
	void RecursivelyDisplayObjects(eastl::vector<TransformObjPtr>& inObjects, const bool inDisplayNode);

public:
	eastl::vector<TransformObjPtr> Objects;
	eastl::shared_ptr<class Camera> CurrentCamera;
};