#include "AssimpModel3D.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Logger/Logger.h"
#include "assimp/postprocess.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/ForwardRenderer.h"
#include "Renderer/RenderingPrimitives.h"
#include "Renderer/RHI/Resources/RenderDataContainer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer/RHI/Resources/RHITexture.h"
#include "Renderer/RHI/RHI.h"

AssimpModel3D::AssimpModel3D(const eastl::string& inPath)
	: ModelPath{ inPath }
{}

AssimpModel3D::~AssimpModel3D() = default;

void AssimpModel3D::CreateProxy()
{
	eastl::shared_ptr<AssimpModel3D> thisShared = this_shared(this);

	// TODO: Make this work with multithreaded again
	//RenderingLoadCommand loadCommand;
	//loadCommand.LoadDel = LoadRenderResourceDelegate::CreateStatic(LoadModelToRoot);
	//loadCommand.ModelPath = ModelPath;
	//loadCommand.Parent = thisShared;

	LoadModelToRoot(ModelPath, thisShared);
}

class AssimpModel3DLoader
{
protected:
	AssimpModel3DLoader(const eastl::string& inPath);
	~AssimpModel3DLoader();

private:
	eastl::shared_ptr<MeshNode> LoadData(OUT eastl::vector<RenderCommand>& outCommands);
	void ProcessNodesRecursively(const struct aiNode& inNode, const struct aiScene& inScene, eastl::shared_ptr<MeshNode>& inCurrentNode, OUT eastl::vector<RenderCommand>& outCommands);
	void ProcessMesh(const struct aiMesh& inMesh, const struct aiScene& inScene, eastl::shared_ptr<MeshNode>& inCurrentNode, OUT eastl::vector<RenderCommand>& outCommands);

	eastl::vector<eastl::shared_ptr<class RHITexture2D>> LoadMaterialTextures(const struct aiMaterial& inMat, const aiTextureType& inAssimpTexType);
	bool IsTextureLoaded(const eastl::string& inTexPath, OUT eastl::shared_ptr<class RHITexture2D>& outTex);

	static Transform aiMatrixToTransform(const aiMatrix4x4& inMatrix);
private:
	eastl::vector<eastl::shared_ptr<class RHITexture2D>> LoadedTextures;
	eastl::string ModelDir;
	eastl::string ModelPath;

	friend class AssimpModel3D;
};

void AssimpModel3D::LoadModelToRoot(const eastl::string inPath, TransformObjPtr inParent)
{
	AssimpModel3DLoader modelLoader(inPath);

	eastl::vector<RenderCommand> resultingCommands;
	eastl::shared_ptr<MeshNode> mesh = modelLoader.LoadData(resultingCommands);

	ForwardRenderer::Get().AddCommands(resultingCommands);

	inParent->AddChild(mesh);
}

AssimpModel3DLoader::AssimpModel3DLoader(const eastl::string & inPath)
	: ModelPath{ inPath }
{}

AssimpModel3DLoader::~AssimpModel3DLoader() = default;

eastl::shared_ptr<MeshNode> AssimpModel3DLoader::LoadData(OUT eastl::vector<RenderCommand>& outCommands)
{
	Assimp::Importer modelImporter;

	const aiScene* scene = modelImporter.ReadFile(ModelPath.c_str(), 0);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Unable to load model from path %s", ModelPath.c_str());

		return nullptr;
	}

	ModelDir = ModelPath.substr(0, ModelPath.find_last_of('/'));

	eastl::shared_ptr<MeshNode> newNode = eastl::make_shared<MeshNode>();
	newNode->SetRelTransform(aiMatrixToTransform(scene->mRootNode->mTransformation));
	ProcessNodesRecursively(*scene->mRootNode, *scene, newNode, outCommands);

	return newNode;
}

void AssimpModel3DLoader::ProcessNodesRecursively(const aiNode & inNode, const aiScene & inScene, eastl::shared_ptr<MeshNode>& inCurrentNode, OUT eastl::vector<RenderCommand>& outCommands)
{
	for (uint32_t i = 0; i < inNode.mNumMeshes; ++i)
	{
		const uint32_t meshIndex = inNode.mMeshes[i];
		const aiMesh* assimpMesh = inScene.mMeshes[meshIndex];

		ProcessMesh(*assimpMesh, inScene, inCurrentNode, outCommands);
	}

	for (uint32_t i = 0; i < inNode.mNumChildren; ++i)
	{
		const aiNode& nextAiNode = *inNode.mChildren[i];
		eastl::shared_ptr<MeshNode> newNode = eastl::make_shared<MeshNode>();
		newNode->SetRelTransform(aiMatrixToTransform(nextAiNode.mTransformation));

		ProcessNodesRecursively(nextAiNode, inScene, newNode, outCommands);
		inCurrentNode->AddChild((newNode));
	}
}

void AssimpModel3DLoader::ProcessMesh(const aiMesh& inMesh, const aiScene& inScene, eastl::shared_ptr<MeshNode>& inCurrentNode, OUT eastl::vector<RenderCommand>& outCommands)
{
	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Normals
	inputLayout.Push<float>(3, VertexInputType::Normal);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);

 	MaterialsManager& matManager = MaterialsManager::Get();
 	bool materialExists = false;
 	eastl::shared_ptr<RenderMaterial> thisMaterial = matManager.GetOrAddMaterial(eastl::string("Assimp_Material_") + inMesh.mName.data, materialExists);
 
 	if (!materialExists)
 	{
		eastl::vector<RHITexture2D> textures;
		if (inMesh.mMaterialIndex >= 0)
		{
			aiMaterial* Material = inScene.mMaterials[inMesh.mMaterialIndex];
			eastl::vector<eastl::shared_ptr<RHITexture2D>> diffuseMaps = LoadMaterialTextures(*Material, aiTextureType_DIFFUSE);
			thisMaterial->DiffuseTextures.insert(thisMaterial->DiffuseTextures.end(), eastl::make_move_iterator(diffuseMaps.begin()), eastl::make_move_iterator(diffuseMaps.end()));

			// 		std::vector<Texture> SpecularMaps = LoadMaterialTextures(Material, aiTextureType_SPECULAR, TextureType::Specular);
			// 		Textures.insert(Textures.end(), SpecularMaps.begin(), SpecularMaps.end());
		}

		thisMaterial->Shader = RHI::Instance->CreateShaderFromPath("ModelWorldPosition_VS_Pos-Normal-UV", "BasicTex_PS", inputLayout);
 	}
 
	const eastl::string renderDataContainerID = inMesh.mName.C_Str();
	eastl::shared_ptr<RenderDataContainer> dataContainer;
	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(renderDataContainerID, dataContainer);

	if (!existingContainer)
	{
		eastl::vector<Vertex> vertices;
		eastl::vector<uint32_t> indices;

		for (uint32_t i = 0; i < inMesh.mNumVertices; i++)
		{
			Vertex vert;
			aiVector3D aiVertex = inMesh.mVertices[i];
			aiVector3D aiNormal = inMesh.mNormals[i];
			vert.Position = glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z);
			vert.Normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);

			if (inMesh.mTextureCoords[0])
			{
				aiVector3D aiTexCoords = inMesh.mTextureCoords[0][i];

				glm::vec2 Coords;
				Coords.x = aiTexCoords.x;
				Coords.y = aiTexCoords.y;

				vert.TexCoords = Coords;
			}
			else
			{
				vert.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vert);
		}

		for (uint32_t i = 0; i < inMesh.mNumFaces; i++)
		{
			aiFace Face = inMesh.mFaces[i];

			for (uint32_t j = 0; j < Face.mNumIndices; j++)
			{
				indices.push_back(Face.mIndices[j]);
			}
		}

		const int32_t indicesCount = static_cast<int32_t>(indices.size());
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Instance->CreateIndexBuffer(indices.data(), indicesCount);
		const int32_t verticesCount = static_cast<int32_t>(vertices.size());
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Instance->CreateVertexBuffer(inputLayout, vertices, ib);

		dataContainer->VBuffer = vb;
	}

	eastl::shared_ptr<MeshNode> newMesh = eastl::make_shared<MeshNode>();
	inCurrentNode->AddChild(newMesh);

	RenderCommand newCommand;
	newCommand.Material = thisMaterial;
	newCommand.Parent = inCurrentNode;
	newCommand.DataContainer = dataContainer;
	newCommand.DrawType = EDrawCallType::DrawElements;

	outCommands.push_back(newCommand);
}

eastl::vector<eastl::shared_ptr<RHITexture2D>> AssimpModel3DLoader::LoadMaterialTextures(const aiMaterial& inMat, const aiTextureType& inAssimpTexType)
{
	eastl::vector<eastl::shared_ptr<RHITexture2D>> textures;
	const uint32_t texureBaseNr = GL_TEXTURE0;
	const uint32_t texturesCount = inMat.GetTextureCount(inAssimpTexType);

	for (uint32_t i = 0; i < texturesCount; ++i)
	{
		aiString Str;
		inMat.GetTexture(inAssimpTexType, i, &Str);
		eastl::shared_ptr<RHITexture2D> tex = nullptr;

		if (!IsTextureLoaded(Str.C_Str(), tex))
		{
			const eastl::string path = ModelDir + eastl::string("/") + eastl::string(Str.C_Str());
			tex = RHI::Instance->CreateTexture2D(path);
			tex->SourcePath = eastl::string(Str.C_Str());
			LoadedTextures.push_back(tex);
		}

		textures.push_back(tex);
	}

	return textures;
}

bool AssimpModel3DLoader::IsTextureLoaded(const eastl::string& inTexPath, OUT eastl::shared_ptr<RHITexture2D>& outTex)
{
	for (const eastl::shared_ptr<RHITexture2D>& loadedTexture : LoadedTextures)
	{
		if (loadedTexture->SourcePath == inTexPath)
		{
			outTex = loadedTexture;
			return true;
		}
	}

	return false;
}

Transform AssimpModel3DLoader::aiMatrixToTransform(const aiMatrix4x4 & inMatrix)
{
	aiVector3D aiScaling;
	aiVector3D aiRotation;
	aiVector3D aiPosition;
	inMatrix.Decompose(aiScaling, aiRotation, aiPosition);

	glm::vec3 scaling(aiScaling.x, aiScaling.y, aiScaling.z);
	glm::vec3 rotation(aiRotation.x, aiRotation.y, aiRotation.z);
	glm::vec3 translation(aiPosition.x, aiPosition.y, aiPosition.z);

	return Transform(translation, rotation, scaling);
}
