#include "AssetManager.h"

AssetManager::AssetManager() = default;
AssetManager::~AssetManager() = default;

void AssetManager::LoadAssets()
{
	LoadMaterials();
}

void AssetManager::LoadMaterials()
{
	// TODO: Hardcoded, this should be serialized from folder containing the assets in their format and from .pak file later
	//MaterialsManager& materManager = MaterialsManager::Get();


}

