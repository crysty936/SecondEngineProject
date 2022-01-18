#include <assert.h>
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLUtils.h"
#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Renderer/OpenGL/OpenGLRenderableObject.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Core/IGameObject.h"

#define CLEAR_COLOR 0.3f, 0.5f, 1.f, 0.4f

OpenGLRenderer* RHI = nullptr;

OpenGLRenderer::OpenGLRenderer(const WindowProperties& inDefaultWindowProperties)
{
	const bool glfwSuccess = glfwInit() == GLFW_TRUE;

	ASSERT(glfwSuccess);

	glfwSetErrorCallback(OpenGLUtils::GLFWErrorCallback);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	// Create new Window for data holding
	MainWindow = CreateWindow(inDefaultWindowProperties);

	Perspective = glm::perspective(glm::radians(45.0f), (float)MainWindow->GetProperties().Width / (float)MainWindow->GetProperties().Height, 0.1f, 100.0f);

	// Set Context
	GLFWwindow* mainWindowHandle = MainWindow->GetHandle();
	glfwMakeContextCurrent(mainWindowHandle);
	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;
	glfwSetInputMode(mainWindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, inDefaultWindowProperties.Width, inDefaultWindowProperties.Height);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);
	glClearColor(CLEAR_COLOR);
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Init(const WindowProperties & inDefaultWindowProperties)
{
	RHI = new OpenGLRenderer{ inDefaultWindowProperties };
}

void OpenGLRenderer::Terminate()
{
	RHI->MainWindow.reset();
	glfwTerminate();

	ASSERT(RHI);
	delete RHI;
}

void OpenGLRenderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	SceneManager& sceneMan = SceneManager::Get();
	Scene& currentScene = sceneMan.GetCurrentScene();
	eastl::vector<eastl::shared_ptr<IGameObject>>& sceneObjects = currentScene.SceneObjects;

	constexpr glm::mat4 identity = glm::mat4(1.f);
	RecursiveDrawObjects(sceneObjects, identity);

	CheckShouldCloseWindow(*MainWindow);
	glfwSwapBuffers(MainWindow->GetHandle());
}

void OpenGLRenderer::RecursiveDrawObjects(eastl::vector<eastl::shared_ptr<IGameObject>>& inObjects, const glm::mat4 inParentModel)
{
	glm::mat4 view = glm::mat4(1.0f);
	// we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	for (eastl::shared_ptr<IGameObject>& object : inObjects)
	{
		IGameObject* tickable = object.get();
		tickable->Model.Translation.x += 0.01f;
		OpenGLRenderableObject* renderable = dynamic_cast<OpenGLRenderableObject*>(tickable);
		glm::mat4 currentModel = tickable->Model.GetModel();

		RecursiveDrawObjects(tickable->Children, currentModel);

		if (renderable)
		{
			// Bind and set all uniforms
			OpenGLShader& shader = renderable->GetShader();
			shader.Bind();

			shader.SetUniformValue4fv("model", currentModel);
			shader.SetUniformValue4fv("projection", Perspective);
			shader.SetUniformValue4fv("view", view);

			// Draw using shader
			renderable->Draw();
		}
	}
}

eastl::unique_ptr<OpenGLWindow> OpenGLRenderer::CreateWindow(const WindowProperties & inWindowProperties) const
{
	GLFWwindow* newHandle = CreateNewWindowHandle(inWindowProperties);
	eastl::unique_ptr<OpenGLWindow> newWindow = eastl::make_unique<OpenGLWindow>(newHandle);

	return newWindow;
}

void OpenGLRenderer::DestroyWindow(GLFWwindow * inWindowHandle) const
{
	glfwDestroyWindow(inWindowHandle);
}

void OpenGLRenderer::SetVSyncEnabled(const bool inEnabled)
{
	glfwSwapInterval(inEnabled);
}

GLFWwindow* OpenGLRenderer::CreateNewWindowHandle(const WindowProperties & inWindowProperties) const
{
	GLFWwindow* newWindowHandle = glfwCreateWindow(inWindowProperties.Width, inWindowProperties.Height, inWindowProperties.Title.data(), nullptr, nullptr);

	return newWindowHandle;
}

void OpenGLRenderer::CheckShouldCloseWindow(const OpenGLWindow & inWindow)
{
	if (glfwWindowShouldClose(inWindow.GetHandle()))
	{
		StopEngineRunning();
	}
}

