#include "BasicShapes.h"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Renderer/OpenGL/Buffer/IndexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexBufferLayout.h"
#include "Renderer/OpenGL/Buffer/VertexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexArrayObject.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/Model/3D/Mesh3D.h"

const uint32_t texureBaseNr = GL_TEXTURE0;

// eastl::shared_ptr<DrawableBase> BasicShapes::CreateTriangleObject(eastl::string inTexturePath)
// {
// 	if (inTexturePath.empty())
// 	{
// 		inTexturePath = eastl::string("../Data/Textures/ExampleContainer.jpg");
// 	}
// 
// 	eastl::shared_ptr<SimpleShapeDrawable> obj = eastl::make_shared<TriangleShape>(inTexturePath);
// 
// 	return obj;
// }
// 
// TriangleShape::TriangleShape(const eastl::string& inTexturePath)
// {
// 	IndexBuffer ibo = IndexBuffer{};
// 	int32_t indicesCount = BasicShapesData::GetTriangleIndicesCount();
// 	ibo.SetIndices(BasicShapesData::GetTriangleIndices(), indicesCount, GL_STATIC_DRAW);
// 
// 	VertexBufferLayout layout = VertexBufferLayout{};
// 	// Vertex points
// 	layout.Push<float>(3);
// 	// Vertex Tex Coords
// 	layout.Push<float>(2);
// 
// 	VertexBuffer vbo = VertexBuffer{ ibo, layout };
// 	int32_t verticesCount = BasicShapesData::GetTriangleVerticesCount();
// 	vbo.SetVertices(BasicShapesData::GetTriangleVertices(), verticesCount, GL_STATIC_DRAW);
// 
// 	ObjectVAO.VBuffer = vbo;
// 	ObjectVAO.SetupState();
// 
// 	Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
// 
// 	OpenGLTexture tex{ inTexturePath, texureBaseNr + 0 };
// 	AddTexture(tex);
// }
// 
// TriangleShape::~TriangleShape() = default;

// eastl::shared_ptr<DrawableBase> BasicShapes::CreateSquareObject(eastl::string inTexturePath)
// {
// 	if (inTexturePath.empty())
// 	{
// 		inTexturePath = eastl::string("../Data/Textures/ExampleContainer.jpg");
// 	}
// 
// 	eastl::shared_ptr<SimpleShapeDrawable> obj = eastl::make_shared<SquareShape>(inTexturePath);
// 
// 	return obj;
// }
// 
 eastl::shared_ptr<DrawableBase> BasicShapes::CreateCubeObject(eastl::string inTexturePath /*= {}*/)
 {
 	if (inTexturePath.empty())
 	{
 		inTexturePath = eastl::string("../Data/Textures/ExampleContainer.jpg");
 	}
 
 	eastl::shared_ptr<CubeShape> obj = eastl::make_shared<CubeShape>(inTexturePath);
 
 	return obj;
 }
// 
// SquareShape::SquareShape(const eastl::string& inTexturePath)
// {
// 	IndexBuffer ibo = IndexBuffer{};
// 	int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
// 	ibo.SetIndices(BasicShapesData::GetSquareIndices(), indicesCount, GL_STATIC_DRAW);
// 
// 	VertexBufferLayout layout = VertexBufferLayout{};
// 	// Vertex points
// 	layout.Push<float>(3);
// 	// Vertex Tex Coords
// 	layout.Push<float>(2);
// 
// 	VertexBuffer vbo = VertexBuffer{ ibo, layout };
// 	int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
// 	vbo.SetVertices(BasicShapesData::GetSquareVertices(), verticesCount, GL_STATIC_DRAW);
// 
// 	ObjectVAO.VBuffer = vbo;
// 	ObjectVAO.SetupState();
// 
// 	Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
// 
// 	OpenGLTexture tex{ inTexturePath, texureBaseNr + 0 };
// 	AddTexture(tex);
// }
// 
// SquareShape::~SquareShape() = default;

CubeShape::CubeShape(const eastl::string& inTexturePath /*= {}*/)
{
	IndexBuffer ibo = IndexBuffer{};
	int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
	ibo.SetIndices(BasicShapesData::GetCubeIndices(), indicesCount, GL_STATIC_DRAW);

	VertexBufferLayout layout = VertexBufferLayout{};
	// Vertex points
	layout.Push<float>(3);
	// Vertex Tex Coords
	layout.Push<float>(2);

	VertexBuffer vbo = VertexBuffer{ ibo, layout };
	int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
	vbo.SetVertices(BasicShapesData::GetCubeVertices(), verticesCount, GL_STATIC_DRAW);

	Mesh3D newMesh;
	newMesh.DrawType = MeshType::DrawArrays;
	newMesh.ObjectVAO.VBuffer = vbo;
	newMesh.ObjectVAO.SetupState();
	OpenGLTexture tex{ inTexturePath, texureBaseNr + 0 };
	newMesh.Textures.push_back(tex);

	AddMesh(eastl::move(newMesh));

	Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
}

CubeShape::~CubeShape() = default;

void CubeShape::Draw_Private() const
{
	for (const Mesh3D& childMesh : Meshes)
	{
		childMesh.Draw(Shader);
	}
}
