#include "MaterialsManager.h"
#include "RenderMaterial.h"
#include "Core/EngineUtils.h"

MaterialsManager* MaterialsManager::Instance = nullptr;

MaterialsManager::MaterialsManager() = default;
MaterialsManager::~MaterialsManager() = default;

void MaterialsManager::Init()
{
	ASSERT_MSG(!Instance);

	Instance = new MaterialsManager();
}

void MaterialsManager::Terminate()
{
	ASSERT_MSG(Instance);

	delete Instance;
}

MaterialsManager& MaterialsManager::Get()
{
	ASSERT_MSG(Instance);

	return *Instance;
}

eastl::shared_ptr<RenderMaterial>& MaterialsManager::GetMaterial(const eastl::string& inMaterialID)
{
	using materialsIterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<RenderMaterial>>::iterator;
	const materialsIterator& iter = LoadedMaterials.find(inMaterialID);
	const bool materialExists = iter != LoadedMaterials.end();

	ASSERT_MSG(materialExists);

	// TODO: If material does not exist, return placeholder material

	eastl::shared_ptr<RenderMaterial>& foundMaterial = (*iter).second;

	return foundMaterial;
}

eastl::shared_ptr<RenderMaterial> MaterialsManager::GetOrAddMaterial(const eastl::string inMaterialID, OUT bool& outAlreadyExists)
{
	using materialsIterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<RenderMaterial>>::iterator;
	const materialsIterator& iter = LoadedMaterials.find(inMaterialID);
	outAlreadyExists = iter != LoadedMaterials.end();

	if (!outAlreadyExists)
	{
		eastl::shared_ptr<RenderMaterial> returnMaterial = eastl::make_shared<RenderMaterial>();
		LoadedMaterials.emplace(inMaterialID, returnMaterial);
	
		return returnMaterial;
	}

	eastl::shared_ptr<RenderMaterial>& foundMaterial = (*iter).second;

	return foundMaterial;
}
