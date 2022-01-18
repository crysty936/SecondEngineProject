#pragma once
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"

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

public:
	eastl::vector<eastl::shared_ptr<struct IGameObject>> SceneObjects;
	eastl::shared_ptr<class Camera> CurrentCamera;
};