#include "BasicShapes.h"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Renderer/OpenGL/Buffer/IndexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexBufferLayout.h"
#include "Renderer/OpenGL/Buffer/VertexBuffer.h"
#include "Renderer/OpenGL/Buffer/VAO.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/DrawableObject.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

const uint32_t texureBaseNr = GL_TEXTURE0;

eastl::shared_ptr<Entity> BasicShapes::CreateTriangleObject(eastl::string inTexturePath)
{
	if (inTexturePath.empty())
	{
		inTexturePath = eastl::string("../Data/Textures/ExampleContainer.jpg");
	}

	eastl::shared_ptr<DrawableObject> obj = eastl::make_shared<TriangleShape>(inTexturePath);

	return obj;
}

TriangleShape::TriangleShape(const eastl::string& inTexturePath)
{
	IndexBuffer ibo = IndexBuffer{};
	int32_t indicesCount = BasicShapesData::GetTriangleIndicesCount();
	ibo.SetIndices(BasicShapesData::GetTriangleIndices(), indicesCount, GL_STATIC_DRAW);

	VertexBufferLayout layout = VertexBufferLayout{};
	// Vertex points
	layout.Push<float>(3);
	// Vertex Tex Coords
	layout.Push<float>(2);

	VertexBuffer vbo = VertexBuffer{ ibo, layout };
	int32_t verticesCount = BasicShapesData::GetTriangleVerticesCount();
	vbo.SetVertices(BasicShapesData::GetTriangleVertices(), verticesCount, GL_STATIC_DRAW);

	VAO vertexArray = VAO{ vbo };

	OpenGLShader basicShaderProgram = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");

	ObjectVAO = vertexArray;
	Shader = basicShaderProgram;

	OpenGLTexture tex{ inTexturePath, texureBaseNr + 0 };
	AddTexture(tex);
}

TriangleShape::~TriangleShape() = default;

void TriangleShape::Tick(const float inDeltaT)
{

}

eastl::shared_ptr<Entity> BasicShapes::CreateSquareObject(eastl::string inTexturePath)
{
	if (inTexturePath.empty())
	{
		inTexturePath = eastl::string("../Data/Textures/ExampleContainer.jpg");
	}

	eastl::shared_ptr<DrawableObject> obj = eastl::make_shared<SquareShape>(inTexturePath);

	return obj;
}

SquareShape::SquareShape(const eastl::string& inTexturePath)
{
	IndexBuffer ibo = IndexBuffer{};
	int32_t indicesCount = BasicShapesData::GetSquareIndicesCount();
	ibo.SetIndices(BasicShapesData::GetSquareIndices(), indicesCount, GL_STATIC_DRAW);

	VertexBufferLayout layout = VertexBufferLayout{};
	// Vertex points
	layout.Push<float>(3);
	// Vertex Tex Coords
	layout.Push<float>(2);

	VertexBuffer vbo = VertexBuffer{ ibo, layout };
	int32_t verticesCount = BasicShapesData::GetSquareVerticesCount();
	vbo.SetVertices(BasicShapesData::GetSquareVertices(), verticesCount, GL_STATIC_DRAW);

	VAO vertexArray = VAO{ vbo };


	OpenGLShader basicShaderProgram = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");

	ObjectVAO = vertexArray;
	Shader = basicShaderProgram;

	OpenGLTexture tex{ inTexturePath, texureBaseNr + 0 };
	AddTexture(tex);
}

SquareShape::~SquareShape() = default;

void SquareShape::Tick(const float inDeltaT)
{
	//Model.Translation.x += 0.01f;
 	//Model.Rotation.x += 1.f;
	//Model.Scale.x *= 0.99f;
}

