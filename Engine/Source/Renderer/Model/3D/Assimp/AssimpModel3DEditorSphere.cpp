#include "AssimpModel3DEditorSphere.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/Material/EngineMaterials/RenderMaterial_EditorSphere.h"
#include "assimp/mesh.h"

AssimpModel3DEditorSphere::AssimpModel3DEditorSphere(const eastl::string& inPath, const eastl::string& inName)
	: AssimpModel3D(inPath, inName)
{


}

AssimpModel3DEditorSphere::~AssimpModel3DEditorSphere() = default;

eastl::shared_ptr<RHIShader> AssimpModel3DEditorSphere::CreateShaders(const class VertexInputLayout& inLayout) const
{
	eastl::vector<ShaderSourceInput> shaders = {
	{ "EditorSphere/VS_EditorSphere", EShaderType::Sh_Vertex },
	{ "EditorSphere/PS_EditorSphere", EShaderType::Sh_Fragment } };

	return RHI::Get()->CreateShaderFromPath(shaders, inLayout);
}

eastl::shared_ptr<RenderMaterial> AssimpModel3DEditorSphere::CreateMaterial(const aiMesh& inMesh, bool& outMatExists) const
{
	static int32_t instanceCount = 0;
	++instanceCount;

	MaterialsManager& matManager = MaterialsManager::Get();
	eastl::shared_ptr<RenderMaterial> thisMaterial = matManager.GetOrAddMaterial<RenderMaterial_EditorSphere>(eastl::string("Assimp_EditorSphere_Material_") + inMesh.mName.data + eastl::to_string(instanceCount), outMatExists);

	return thisMaterial;
}
