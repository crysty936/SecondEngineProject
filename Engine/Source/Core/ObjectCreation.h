#pragma once
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/Drawable.h"

class ObjectCreation
{
public:
	template<typename T, typename... inParamTypes>
	static eastl::shared_ptr<T> NewObject(inParamTypes... inParams)
	{
		eastl::shared_ptr<T> newObject = eastl::make_shared<T>(std::forward<inParamTypes>(inParams)...);
		SceneManager& sManager = SceneManager::Get();
		Scene& currentScene = sManager.GetCurrentScene();
		currentScene.AddObject(newObject);

		// Compile time type deduction and caching member function pointer VTable index to use if type is IDrawable
		constexpr bool isDrawable = std::is_base_of<IDrawableContainer, T>();

		using func = void(IDrawableContainer::*)();
		constexpr func theFunc = &IDrawableContainer::SetupDrawCommands;

 		if (isDrawable)
 		{
 			T* obj = newObject.get();
 			(obj->*theFunc)();
 		}

		return newObject;
	}
};
