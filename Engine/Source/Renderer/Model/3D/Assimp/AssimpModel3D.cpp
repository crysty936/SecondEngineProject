#include "AssimpModel3D.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Logger/Logger.h"
#include "assimp/postprocess.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/ForwardRenderer.h"
#include "Renderer/RenderingPrimitives.h"
#include "Renderer/RHI/Resources/MeshDataContainer.h"
#include "Renderer/RHI/Resources/RHITexture.h"
#include "Renderer/RHI/RHI.h"
#include "Renderer/Material/EngineMaterials/RenderMaterial_WithShadow.h"

static Transform aiMatrixToTransform(const aiMatrix4x4& inMatrix)
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

AssimpModel3D::AssimpModel3D(const eastl::string& inPath, const eastl::string& inName)
	: Model3D(inName), ModelPath{ inPath }
{}

AssimpModel3D::~AssimpModel3D() = default;

void AssimpModel3D::CreateProxy()
{

	// TODO: Make this work with multithreaded again
	//eastl::shared_ptr<AssimpModel3D> thisShared = this_shared(this);
	//RenderingLoadCommand loadCommand;
	//loadCommand.LoadDel = LoadRenderResourceDelegate::CreateStatic(LoadModelToRoot);
	//loadCommand.ModelPath = ModelPath;
	//loadCommand.Parent = thisShared;
	// Add load command
	
	LoadModelToRoot(ModelPath, shared_from_this());
}

void AssimpModel3D::LoadModelToRoot(const eastl::string inPath, TransformObjPtr inParent)
{
	eastl::vector<RenderCommand> resultingCommands;
	eastl::shared_ptr<MeshNode> mesh = LoadData(resultingCommands);

	ForwardRenderer::Get().AddCommands(resultingCommands);

	inParent->AddChild(mesh);
}

eastl::shared_ptr<MeshNode> AssimpModel3D::LoadData(OUT eastl::vector<RenderCommand>& outCommands)
{
	Assimp::Importer modelImporter;

	const aiScene* scene = modelImporter.ReadFile(ModelPath.c_str(), 0);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG_ERROR("Unable to load model from path %s", ModelPath.c_str());

		return nullptr;
	}

	ModelDir = ModelPath.substr(0, ModelPath.find_last_of('/'));

	eastl::shared_ptr<MeshNode> newNode = eastl::make_shared<MeshNode>("RootNode");
	newNode->SetRelTransform(aiMatrixToTransform(scene->mRootNode->mTransformation));
	ProcessNodesRecursively(*scene->mRootNode, *scene, newNode, outCommands);

	return newNode;
}

void AssimpModel3D::ProcessNodesRecursively(const aiNode & inNode, const aiScene & inScene, eastl::shared_ptr<MeshNode>& inCurrentNode, OUT eastl::vector<RenderCommand>& outCommands)
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
		eastl::shared_ptr<MeshNode> newNode = eastl::make_shared<MeshNode>(nextAiNode.mName.C_Str());
		newNode->SetRelTransform(aiMatrixToTransform(nextAiNode.mTransformation));

		ProcessNodesRecursively(nextAiNode, inScene, newNode, outCommands);
		inCurrentNode->AddChild((newNode));
	}
}

eastl::shared_ptr<RHIShader> AssimpModel3D::CreateShaders(const VertexInputLayout& inLayout) const
{
	eastl::vector<ShaderSourceInput> shaders = {
		{ "AssimpLightingShadow/VS_Pos-UV-Normal-Tangent-Bitangent_Model_WorldPosition_WithShadow", EShaderType::Vertex },
		{ "AssimpLightingShadow/PS_TexNormalMapped_WithShadow", EShaderType::Fragment } };

	return RHI::Get()->CreateShaderFromPath(shaders, inLayout);
}

void AssimpModel3D::AddAdditionalBuffers(eastl::shared_ptr<MeshDataContainer>& inDataContainer) const
{
	// to be inherited for instancing, etc
}

RenderCommand AssimpModel3D::CreateRenderCommand(eastl::shared_ptr<RenderMaterial>& inMaterial, eastl::shared_ptr<MeshNode>& inParent, eastl::shared_ptr<MeshDataContainer>& inDataContainer)
{
	RenderCommand newCommand;
	newCommand.Material = inMaterial;
	newCommand.Parent = inParent;
	newCommand.DataContainer = inDataContainer;
	newCommand.DrawType = EDrawType::DrawElements;
	newCommand.DrawPasses = static_cast<EDrawMode::Type>(EDrawMode::Default /*| EDrawMode::NORMAL_VISUALIZE*/);

	return newCommand;
}

void AssimpModel3D::ProcessMesh(const aiMesh& inMesh, const aiScene& inScene, eastl::shared_ptr<MeshNode>& inCurrentNode, OUT eastl::vector<RenderCommand>& outCommands)
{
	VertexInputLayout inputLayout;
	// Vertex points
	inputLayout.Push<float>(3, VertexInputType::Position);
	// Normals
	inputLayout.Push<float>(3, VertexInputType::Normal);
	// Vertex Tex Coords
	inputLayout.Push<float>(2, VertexInputType::TexCoords);
	// Tangent
	inputLayout.Push<float>(3, VertexInputType::Tangent);
	// Bitangent
	inputLayout.Push<float>(3, VertexInputType::Bitangent);

 	MaterialsManager& matManager = MaterialsManager::Get();
 	bool materialExists = false;
 	eastl::shared_ptr<RenderMaterial> thisMaterial = matManager.GetOrAddMaterial<RenderMaterial_WithShadow>(eastl::string("Assimp_Material_") + inMesh.mName.data, materialExists);
 
 	if (!materialExists)
 	{
		eastl::vector<RHITexture2D> textures;
		if (inMesh.mMaterialIndex >= 0)
		{
			aiMaterial* Material = inScene.mMaterials[inMesh.mMaterialIndex];
			eastl::vector<eastl::shared_ptr<RHITexture2D>> diffuseMaps = LoadMaterialTextures(*Material, aiTextureType_DIFFUSE);
			thisMaterial->OwnedTextures.insert(thisMaterial->OwnedTextures.end(), eastl::make_move_iterator(diffuseMaps.begin()), eastl::make_move_iterator(diffuseMaps.end()));

			// 		std::vector<Texture> SpecularMaps = LoadMaterialTextures(Material, aiTextureType_SPECULAR, TextureType::Specular);
			// 		Textures.insert(Textures.end(), SpecularMaps.begin(), SpecularMaps.end());

			uint32_t totalTextureCount = aiGetMaterialTotalTextureCount(Material);

//  			eastl::vector<eastl::shared_ptr<RHITexture2D>> specMap = LoadMaterialTextures(*Material, aiTextureType_UNKNOWN);
// 			thisMaterial->OwnedTextures.insert(thisMaterial->OwnedTextures.end(), eastl::make_move_iterator(specMap.begin()), eastl::make_move_iterator(specMap.end()));

			eastl::vector<eastl::shared_ptr<RHITexture2D>> normalMaps = LoadMaterialTextures(*Material, aiTextureType_NORMALS);
			thisMaterial->OwnedTextures.insert(thisMaterial->OwnedTextures.end(), eastl::make_move_iterator(normalMaps.begin()), eastl::make_move_iterator(normalMaps.end()));
		}

		thisMaterial->Shader = CreateShaders(inputLayout);
 	}
 
	const eastl::string renderDataContainerID = inMesh.mName.C_Str();
	eastl::shared_ptr<MeshDataContainer> dataContainer;
	const bool existingContainer = ForwardRenderer::Get().GetOrCreateContainer(renderDataContainerID, dataContainer);

	if (!existingContainer)
	{
		eastl::vector<Vertex> vertices;
		eastl::vector<uint32_t> indices;

		for (uint32_t i = 0; i < inMesh.mNumVertices; i++)
		{
			Vertex vert;
			const aiVector3D& aiVertex = inMesh.mVertices[i];
			const aiVector3D& aiNormal = inMesh.mNormals[i];

			vert.Position = glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z);
			vert.Normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);
			if (inMesh.mTangents)
			{
				const aiVector3D& aiTangent = inMesh.mTangents[i];
				vert.Tangent = glm::vec3(aiTangent.x, aiTangent.y, aiTangent.z);
			}
			else
			{
				vert.Tangent = glm::vec3(1.f, 0.f, 0.f);
			}

			if (inMesh.mBitangents)
			{
				const aiVector3D& aiBitangent = inMesh.mBitangents[i];
				vert.Bitangent = glm::vec3(aiBitangent.x, aiBitangent.y, aiBitangent.z);
			}
			else
			{
				vert.Bitangent = glm::vec3(0.f, 1.f, 0.f);
			}

			if (inMesh.mTextureCoords[0])
			{
				const aiVector3D& aiTexCoords = inMesh.mTextureCoords[0][i];
				vert.TexCoords = glm::vec2(aiTexCoords.x, aiTexCoords.y);
			}
			else
			{
				vert.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vert);
		}

		for (uint32_t i = 0; i < inMesh.mNumFaces; i++)
		{
			const aiFace& Face = inMesh.mFaces[i];

			for (uint32_t j = 0; j < Face.mNumIndices; j++)
			{
				indices.push_back(Face.mIndices[j]);
			}
		}

		const int32_t indicesCount = static_cast<int32_t>(indices.size());
		eastl::shared_ptr<RHIIndexBuffer> ib = RHI::Get()->CreateIndexBuffer(indices.data(), indicesCount);
		const int32_t verticesCount = static_cast<int32_t>(vertices.size());
		const eastl::shared_ptr<RHIVertexBuffer> vb = RHI::Get()->CreateVertexBuffer(inputLayout, vertices, ib);

		dataContainer->VBuffer = vb;
	}

	AddAdditionalBuffers(dataContainer);

	eastl::shared_ptr<MeshNode> newMesh = eastl::make_shared<MeshNode>(inMesh.mName.C_Str());
	inCurrentNode->AddChild(newMesh);

	RenderCommand newCommand = CreateRenderCommand(thisMaterial, inCurrentNode, dataContainer);
	outCommands.push_back(newCommand);
}

eastl::vector<eastl::shared_ptr<RHITexture2D>> AssimpModel3D::LoadMaterialTextures(const aiMaterial& inMat, const aiTextureType& inAssimpTexType)
{
	eastl::vector<eastl::shared_ptr<RHITexture2D>> textures;
	const uint32_t texturesCount = inMat.GetTextureCount(inAssimpTexType);

	for (uint32_t i = 0; i < texturesCount; ++i)
	{
		aiString Str;
		inMat.GetTexture(inAssimpTexType, i, &Str);
		eastl::shared_ptr<RHITexture2D> tex = nullptr;

		if (!IsTextureLoaded(Str.C_Str(), tex))
		{
			const eastl::string path = ModelDir + eastl::string("/") + eastl::string(Str.C_Str());
			tex = RHI::Get()->CreateAndLoadTexture2D(path, inAssimpTexType != aiTextureType_NORMALS);
			tex->SourcePath = eastl::string(Str.C_Str());
			LoadedTextures.push_back(tex);
		}

		textures.push_back(tex);
	}

	return textures;
}

bool AssimpModel3D::IsTextureLoaded(const eastl::string& inTexPath, OUT eastl::shared_ptr<RHITexture2D>& outTex)
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
