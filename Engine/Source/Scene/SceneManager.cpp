#include "SceneManager.h"
#include "Core/EngineUtils.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Renderer/OpenGL/Buffer/IndexBuffer.h"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Renderer/OpenGL/Buffer/VertexBufferLayout.h"
#include "Renderer/OpenGL/Buffer/VertexBuffer.h"
#include "Renderer/OpenGL/Buffer/VAO.h"
#include "Renderer/RenderableObject.h"
#include "Renderer/OpenGL/OpenGLShader.h"

SceneManager* SceneManager::Instance = nullptr;

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;

void SceneManager::Init()
{
	Instance = new SceneManager{};
}

void SceneManager::LoadScene()
{
	// TODO Should be implemented to read the scene from a serialized output

	Instance->CurrentScene = eastl::make_unique<class Scene>();
	Instance->CurrentScene->SceneObjects.push_back(eastl::make_shared<Camera>());


	IndexBuffer ibo = IndexBuffer{};
	int32_t indicesCount = sizeof(BasicShapes::TriangleIndices) / sizeof(int32_t);
	ibo.SetIndices(BasicShapes::TriangleIndices, indicesCount, GL_STATIC_DRAW);

	VertexBufferLayout layout = VertexBufferLayout{};
	layout.Push<float>(3);

	VertexBuffer vbo = VertexBuffer{ ibo, layout };
	int32_t verticesCount = sizeof(BasicShapes::TriangleVertices) / sizeof(float);
	vbo.SetVertices(BasicShapes::TriangleVertices, verticesCount, GL_STATIC_DRAW);

	VAO vertexArray = VAO{ vbo };

	OpenGLShader basicShaderProgram = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicVertexShader.glsl", "../Data/Shaders/BasicFragmentShader.glsl");

	eastl::shared_ptr<RenderableObject> obj = eastl::make_shared<RenderableObject>(vertexArray, basicShaderProgram);
	Instance->CurrentScene->SceneObjects.push_back(obj);



}

void SceneManager::Terminate()
{
	ASSERT(Instance);

	delete Instance;
}

