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

private:
	void LoadData(const eastl::string& inPath);
	void ProcessNode(const struct aiNode& inNode, const struct aiScene& inScene, MeshNode& inCurrentNode);
	void ProcessMesh(const struct aiMesh& inMesh, const struct aiScene& inScene, MeshNode& inCurrentNode);

	eastl::vector<OpenGLTexture> AssimpModel3D::LoadMaterialTextures(const aiMaterial& inMat, const aiTextureType& inAssimpTexType, const TextureType inTexType);
	bool IsTextureLoaded(const eastl::string& inTexPath, OUT class OpenGLTexture& outTex);
private:
	eastl::vector<OpenGLTexture> LoadedTextures;
	eastl::string ModelDir;

};