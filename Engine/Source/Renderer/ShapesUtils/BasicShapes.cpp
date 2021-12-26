#include "BasicShapes.h"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Renderer/OpenGL/Buffer/IndexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexBufferLayout.h"
#include "Renderer/OpenGL/Buffer/VertexBuffer.h"
#include "Renderer/OpenGL/Buffer/VAO.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/RenderableObject.h"

eastl::shared_ptr<RenderableObject> BasicShapes::GetTriangleRenderable()
{
	IndexBuffer ibo = IndexBuffer{};
	int32_t indicesCount = sizeof(BasicShapesData::TriangleIndices) / sizeof(int32_t);
	ibo.SetIndices(BasicShapesData::TriangleIndices, indicesCount, GL_STATIC_DRAW);

	VertexBufferLayout layout = VertexBufferLayout{};
	layout.Push<float>(3);

	VertexBuffer vbo = VertexBuffer{ ibo, layout };
	int32_t verticesCount = sizeof(BasicShapesData::TriangleVertices) / sizeof(float);
	vbo.SetVertices(BasicShapesData::TriangleVertices, verticesCount, GL_STATIC_DRAW);

	VAO vertexArray = VAO{ vbo };

	OpenGLShader basicShaderProgram = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicFragmentShader.glsl");

	eastl::shared_ptr<RenderableObject> obj = eastl::make_shared<RenderableObject>(vertexArray, basicShaderProgram);
	
	return obj;
}

eastl::shared_ptr<RenderableObject> BasicShapes::GetSquareRenderable()
{
	IndexBuffer ibo = IndexBuffer{};
	int32_t indicesCount = sizeof(BasicShapesData::SquareIndices) / sizeof(int32_t);
	ibo.SetIndices(BasicShapesData::SquareIndices, indicesCount, GL_STATIC_DRAW);

	VertexBufferLayout layout = VertexBufferLayout{};
	layout.Push<float>(3);

	VertexBuffer vbo = VertexBuffer{ ibo, layout };
	int32_t verticesCount = sizeof(BasicShapesData::SquareVertices) / sizeof(float);
	vbo.SetVertices(BasicShapesData::SquareVertices, verticesCount, GL_STATIC_DRAW);

	VAO vertexArray = VAO{ vbo };

	OpenGLShader basicShaderProgram = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicFragmentShader.glsl");

	eastl::shared_ptr<RenderableObject> obj = eastl::make_shared<RenderableObject>(vertexArray, basicShaderProgram);

	return obj;
}
