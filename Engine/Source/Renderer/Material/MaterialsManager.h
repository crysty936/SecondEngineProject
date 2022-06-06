#pragma once
#include "EASTL/unordered_map.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/string.h"
#include "Core/EngineUtils.h"

class MaterialsManager
{
private:
	MaterialsManager();
	~MaterialsManager();

public:
	static void Init();
	static void Terminate();
	static MaterialsManager& Get();
	eastl::shared_ptr<class RenderMaterial>& GetMaterial(const eastl::string& inMaterialID);

public:
	// TODO: This should preferrably be removed, materials should only be added by the Asset Manager at startup once deserialization is done
	eastl::shared_ptr<class RenderMaterial> GetOrAddMaterial(const eastl::string& inMaterialID, OUT bool& outAlreadyExists);

private:
	eastl::unordered_map<eastl::string, eastl::shared_ptr<class RenderMaterial>> LoadedMaterials{};
	static MaterialsManager* Instance;
};

