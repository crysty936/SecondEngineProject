#include "BasicShapes.h"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Renderer/OpenGL/Buffer/IndexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexBufferLayout.h"
#include "Renderer/OpenGL/Buffer/VertexBuffer.h"
#include "Renderer/OpenGL/Buffer/VAO.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/RenderableModel.h"
#include "Renderer/OpenGL/OpenGLTexture.h"

const uint32_t texureBaseNr = GL_TEXTURE0;

eastl::shared_ptr<RenderableModel> BasicShapes::GetTriangleRenderable()
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

	eastl::shared_ptr<RenderableModel> model = eastl::make_shared<RenderableModel>(vertexArray, basicShaderProgram);

	OpenGLTexture tex{ "../Data/Textures/ExampleContainer.jpg", texureBaseNr + 0 };
	model->AddTexture(tex);
	
	return model;
}

eastl::shared_ptr<RenderableModel> BasicShapes::GetSquareRenderable()
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

	eastl::shared_ptr<RenderableModel> model = eastl::make_shared<RenderableModel>(vertexArray, basicShaderProgram);

	OpenGLTexture tex{ "../Data/Textures/ExampleContainer.jpg", texureBaseNr + 0 };
	model->AddTexture(tex);

	return model;
}
