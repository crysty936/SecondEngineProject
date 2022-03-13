#pragma once
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/IDrawable.h"

class ObjectCreation
{
public:
	template<typename T, typename... inParamTypes>
	static eastl::shared_ptr<T> CreateEntity(inParamTypes... inParams)
	{
		eastl::shared_ptr<T> newObject = eastl::make_shared<T>(std::forward<inParamTypes>(inParams)...);
		SceneManager& sManager = SceneManager::Get();
		Scene& currentScene = sManager.GetCurrentScene();
		currentScene.AddObject(newObject);

		return newObject;
	}


	template<typename T, typename... inParamTypes>
	static eastl::shared_ptr<T> CreateDrawable(inParamTypes... inParams)
	{
		eastl::shared_ptr<T> newObject = eastl::make_shared<T>(std::forward<inParamTypes>(inParams)...);
		newObject->SetupDrawCommand();
		SceneManager& sManager = SceneManager::Get();
		Scene& currentScene = sManager.GetCurrentScene();
		currentScene.AddObject(newObject);

		return newObject;
	}

};
