#include "AssimpModel3D.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Logger/Logger.h"

AssimpModel3D::AssimpModel3D(const eastl::string& inPath)
{
	LoadData(inPath);
}

AssimpModel3D::~AssimpModel3D() = default;

void AssimpModel3D::LoadData(const eastl::string& inPath)
{
	Assimp::Importer modelImporter;

	const aiScene* scene = modelImporter.ReadFile(inPath.c_str(), 0);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Unable to load model from path %s", inPath.c_str());

		return;
	}



	ProcessNode(*scene->mRootNode, *scene);
}



void AssimpModel3D::ProcessNode(const aiNode& inNode, const aiScene& inScene)
{

	for (uint32_t i = 0; i < inNode.mNumMeshes; ++i)
	{
		const uint32_t meshIndex = inNode.mMeshes[i];
		const aiMesh* assimpMesh = inScene.mMeshes[meshIndex];

		
	}

	for (uint32_t i = 0; i < inNode.mNumChildren; ++i)
	{
		ProcessNode(*inNode.mChildren[i], inScene);
	}

}