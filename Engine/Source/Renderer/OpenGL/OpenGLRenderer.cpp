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
#include "Renderer/Drawable/SimpleShapeDrawable.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Entity/Entity.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniformBase.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform4fv.h"

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

	// Set Context
	GLFWwindow* mainWindowHandle = MainWindow->GetHandle();
	glfwMakeContextCurrent(mainWindowHandle);
	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;
	glfwSetInputMode(mainWindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, inDefaultWindowProperties.Width, inDefaultWindowProperties.Height);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 	glEnable(GL_BLEND);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);
	glClearColor(CLEAR_COLOR);

	SetupBaseUniforms();
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
	eastl::vector<eastl::shared_ptr<Entity>>& sceneObjects = currentScene.Entities;

	UpdateUniforms();
	RecursiveDrawObjects(sceneObjects);
	constexpr glm::mat4 identity = glm::mat4(1.f);

	CheckShouldCloseWindow(*MainWindow);
	glfwSwapBuffers(MainWindow->GetHandle());
}

void OpenGLRenderer::SetupBaseUniforms()
{
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)MainWindow->GetProperties().Width / (float)MainWindow->GetProperties().Height, 0.1f, 1000.0f);
	UniformsCache["projection"] = projection;
}

void OpenGLRenderer::UpdateUniforms()
{
 	const glm::mat4 view = SceneManager::Get().GetCurrentScene().CurrentCamera->GetLookAt();
	UniformsCache["view"] = view;
}

void OpenGLRenderer::RecursiveDrawObjects(const eastl::vector<eastl::shared_ptr<Entity>>& inObjects)
{
	for (const eastl::shared_ptr<Entity>& object : inObjects)
	{
		const Entity* entt = object.get();
		RecursiveDrawObjects(entt->GetChildren());

		const DrawableBase* drawable = dynamic_cast<const DrawableBase*>(entt);
		if (drawable)
		{
			drawable->Draw(UniformsCache);
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

