#pragma once
#include "Renderer/Model/3D/Model3D.h"
#include "Renderer/Model/3D/Mesh3D.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "EASTL/string.h"
#include "Core/EngineUtils.h"
#include "assimp/material.h"

class AssimpModel3D : public Model3D
{
public:
	AssimpModel3D(const eastl::string& inPath);
	virtual ~AssimpModel3D();

	virtual void SetupDrawCommand() override;

private:
	void LoadData(const eastl::string& inPath);
	void ProcessNode(const struct aiNode& inNode, const struct aiScene& inScene, eastl::shared_ptr<MeshNode>& inCurrentNode);
	void ProcessMesh(const struct aiMesh& inMesh, const struct aiScene& inScene, eastl::shared_ptr<MeshNode>& inCurrentNode);

	eastl::vector<OpenGLTexture> AssimpModel3D::LoadMaterialTextures(const struct aiMaterial& inMat, const aiTextureType& inAssimpTexType, const TextureType inTexType);
	bool IsTextureLoaded(const eastl::string& inTexPath, OUT class OpenGLTexture& outTex);

	static Transform aiMatrixToTransform(const aiMatrix4x4& inMatrix);
private:
	eastl::vector<OpenGLTexture> LoadedTextures{};
	eastl::string ModelDir{};
	eastl::string ModelPath{};
};