#include "MaterialsManager.h"
#include "RenderMaterial.h"
#include "Core/EngineUtils.h"

MaterialsManager* MaterialsManager::Instance = nullptr;

MaterialsManager::MaterialsManager() = default;
MaterialsManager::~MaterialsManager() = default;

void MaterialsManager::Init()
{
	ASSERT(!Instance);

	Instance = new MaterialsManager();
}

void MaterialsManager::Terminate()
{
	ASSERT(Instance);

	delete Instance;
}

MaterialsManager& MaterialsManager::Get()
{
	ASSERT(Instance);

	return *Instance;
}

eastl::shared_ptr<RenderMaterial>& MaterialsManager::GetMaterial(const eastl::string& inMaterialID)
{
	using materialsIterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<RenderMaterial>>::iterator;
	const materialsIterator& iter = LoadedMaterials.find(inMaterialID);
	const bool materialExists = iter != LoadedMaterials.end();

	ASSERT(materialExists);

	// TODO: If material does not exist, return placeholder material

	eastl::shared_ptr<RenderMaterial>& foundMaterial = (*iter).second;

	return foundMaterial;
}

eastl::shared_ptr<RenderMaterial> MaterialsManager::GetOrAddMaterial(const eastl::string inMaterialID, OUT bool& outAlreadyExists)
{
	using materialsIterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<RenderMaterial>>::iterator;
	const materialsIterator& iter = LoadedMaterials.find(inMaterialID);
	const bool materialExists = iter != LoadedMaterials.end();

	if (!materialExists)
	{
		eastl::shared_ptr<RenderMaterial> returnMaterial = eastl::make_shared<RenderMaterial>();
		outAlreadyExists = false;
		LoadedMaterials.emplace(inMaterialID, returnMaterial);
	
		return returnMaterial;
	}

	eastl::shared_ptr<RenderMaterial>& foundMaterial = (*iter).second;

	return foundMaterial;
}
