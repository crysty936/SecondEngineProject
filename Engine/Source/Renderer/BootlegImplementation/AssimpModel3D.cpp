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

	ModelDir = inPath.substr(0, inPath.find_last_of('/'));

	ProcessNode(*scene->mRootNode, *scene);
}

void AssimpModel3D::ProcessNode(const aiNode& inNode, const aiScene& inScene)
{

	for (uint32_t i = 0; i < inNode.mNumMeshes; ++i)
	{
		const uint32_t meshIndex = inNode.mMeshes[i];
		const aiMesh* assimpMesh = inScene.mMeshes[meshIndex];

		ProcessMesh(*assimpMesh, inScene);
	}

	for (uint32_t i = 0; i < inNode.mNumChildren; ++i)
	{
		ProcessNode(*inNode.mChildren[i], inScene);
	}
}


void AssimpModel3D::ProcessMesh(const aiMesh& inMesh, const aiScene& inScene)
{
	eastl::vector<Vertex> vertices;
	eastl::vector<uint32_t> indices;
	eastl::vector<OpenGLTexture> textures;

	for (uint32_t i = 0; i < inMesh.mNumVertices; i++)
	{
		Vertex vert;
		aiVector3D aiVertex = inMesh.mVertices[i];
		aiVector3D aiNormal = inMesh.mNormals[i];
		vert.Position = glm::vec3(aiVertex.x, aiVertex.y, aiVertex.z);
		//vert.Normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);

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

	if (inMesh.mMaterialIndex >= 0)
	{
		aiMaterial* Material = inScene.mMaterials[inMesh.mMaterialIndex];
		eastl::vector<OpenGLTexture> DiffuseMaps = LoadMaterialTextures(*Material, aiTextureType_DIFFUSE, TextureType::Diffuse);
		textures.insert(textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());

// 		std::vector<Texture> SpecularMaps = LoadMaterialTextures(Material, aiTextureType_SPECULAR, TextureType::Specular);
// 		Textures.insert(Textures.end(), SpecularMaps.begin(), SpecularMaps.end());
	}

	IndexBuffer ibo = IndexBuffer{};
	int32_t indicesCount = static_cast<int32_t>(indices.size());
	ibo.SetIndices(indices.data(), indicesCount, GL_STATIC_DRAW);

	VertexBufferLayout layout = VertexBufferLayout{};
	// Vertex points
	layout.Push<float>(3);
	// Vertex Tex Coords
	layout.Push<float>(2);
	
	VertexBuffer vbo = VertexBuffer{ ibo, layout };
	int32_t verticesCount = static_cast<int32_t>(vertices.size());
	vbo.SetVertices(vertices, GL_STATIC_DRAW);

	Mesh3D newMesh;
	newMesh.DrawType = MeshType::DrawElements;
	newMesh.ObjectVAO.VBuffer = vbo;
	newMesh.ObjectVAO.SetupState();
// 	OpenGLTexture tex{ inTexturePath, texureBaseNr + 0 };
// 	newMesh.Textures.push_back(tex);

	AddMesh(eastl::move(newMesh));

	Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
}

eastl::vector<OpenGLTexture> AssimpModel3D::LoadMaterialTextures(const aiMaterial& inMat, const aiTextureType& inAssimpTexType, const TextureType inTexType)
{
	eastl::vector<OpenGLTexture> textures;
	const uint32_t texureBaseNr = GL_TEXTURE0;

	for (uint32_t i = 0; i < inMat.GetTextureCount(inAssimpTexType); i++)
	{
		aiString Str;
		inMat.GetTexture(inAssimpTexType, i, &Str);
		OpenGLTexture Tex;

		if (!IsTextureLoaded(Str.C_Str(), Tex))
		{
			eastl::string path = ModelDir + eastl::string("/") + eastl::string(Str.C_Str());
			OpenGLTexture tex(path, texureBaseNr + i);
			Tex.TexType = inTexType;
			Tex.TexPath = eastl::string(Str.C_Str());
			LoadedTextures.push_back(Tex);
		}

		textures.push_back(Tex);
	}

	return textures;
}

bool AssimpModel3D::IsTextureLoaded(const eastl::string& inTexPath, OUT OpenGLTexture& outTex)
{
	for (const OpenGLTexture& loadedTexture : LoadedTextures)
	{
		if (loadedTexture.TexPath == inTexPath)
		{
			outTex = loadedTexture;
			return true;
		}
	}
	return false;
}
