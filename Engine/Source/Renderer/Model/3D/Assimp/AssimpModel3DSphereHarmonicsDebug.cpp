#include "AssimpModel3DSphereHarmonicsDebug.h"
#include "Renderer/Material/MaterialsManager.h"
#include "assimp/mesh.h"
#include "Renderer/Material/EngineMaterials/RenderMaterial_SphereHarmonicsDebug.h"

AssimpModel3DSphereHarmonicsDebug::AssimpModel3DSphereHarmonicsDebug(const eastl::string& inPath, const eastl::string& inName)
	: AssimpModel3D(inPath, inName)
{}

AssimpModel3DSphereHarmonicsDebug::~AssimpModel3DSphereHarmonicsDebug() = default;

eastl::shared_ptr<RHIShader> AssimpModel3DSphereHarmonicsDebug::CreateShaders(const class VertexInputLayout& inLayout) const
{
	eastl::vector<ShaderSourceInput> shaders = {
	{ "SphericalHarmonicsDebug/VS", EShaderType::Sh_Vertex },
	{ "SphericalHarmonicsDebug/PS", EShaderType::Sh_Fragment } };

	return RHI::Get()->CreateShaderFromPath(shaders, inLayout);
}

eastl::shared_ptr<RenderMaterial> AssimpModel3DSphereHarmonicsDebug::CreateMaterial(const aiMesh& inMesh, bool& outMatExists) const
{
	static int32_t instanceCount = 0;
	++instanceCount;

	MaterialsManager& matManager = MaterialsManager::Get();
	eastl::shared_ptr<RenderMaterial> thisMaterial = matManager.GetOrAddMaterial<RenderMaterial_SphereHarmonicsDebug>(eastl::string("Assimp_SphereSHDebug_Material_") + inMesh.mName.data + eastl::to_string(instanceCount), outMatExists);

	return thisMaterial;
}
