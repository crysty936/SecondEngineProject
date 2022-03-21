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
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Entity/Entity.h"
#include "Renderer/Drawable/Drawable.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/Material/RenderMaterial.h"
#include "Buffer/VertexArrayObject.h"
#include "Renderer/Material/MaterialsManager.h"

const glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);

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
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// 	glEnable(GL_BLEND);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);

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
	SceneManager& sceneMan = SceneManager::Get();
	Scene& currentScene = sceneMan.GetCurrentScene();

	UpdateUniforms();
	DrawCommands();
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

void OpenGLRenderer::DrawCommands()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (int32_t commandIndex = 0; commandIndex < Commands.size(); ++commandIndex)
	{
		const RenderCommand& renderCommand = Commands[commandIndex];
		const eastl::shared_ptr<const DrawableObject> parent = renderCommand.Parent.lock();
		if (renderCommand.Parent.expired())
		{
			continue;
		}

		glClear(GL_STENCIL_BUFFER_BIT);

		SetDrawMode(EDrawMode::NORMAL);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		DrawCommand(renderCommand);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		SetDrawMode(EDrawMode::OUTLINE);

		DrawableObject& parentNonConst = const_cast<DrawableObject&>(*parent);

		parentNonConst.SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

		DrawCommand(renderCommand);

		parentNonConst.SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

		glEnable(GL_DEPTH_TEST);
 		glStencilMask(0xFF);
 		glStencilFunc(GL_ALWAYS, 1, 0xFF);
	}
}

void OpenGLRenderer::DrawCommand(const RenderCommand & inCommand)
{
	const eastl::shared_ptr<const DrawableObject> parent = inCommand.Parent.lock();
	const eastl::shared_ptr<RenderMaterial> material = GetMaterial(inCommand);
	const eastl::shared_ptr<VertexArrayObject>& vao = inCommand.VAO;

	if (!parent->IsVisible())
	{
		return;
	}

	// TODO: Abstract the model and parent dependent uniforms (like the Model Matrix) to be present in the render command 
	// and updated only if dirty

	material->Shader.Bind();

	UniformsCache["model"] = parent->GetModelMatrix();

	for (const OpenGLTexture& tex : material->Textures)
	{
		tex.Bind();
	}

	const uint32_t indicesCount = vao->VBuffer.GetIndicesCount();
	vao->Bind();
	material->SetUniforms(UniformsCache);

	switch (inCommand.DrawType)
	{
	case EDrawCallType::DrawElements:
	{
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		break;
	}
	case EDrawCallType::DrawArrays:
	{
		glDrawArrays(GL_TRIANGLES, 0, indicesCount);
		break;
	}
	}

	vao->Unbind();

	for (const OpenGLTexture& tex : material->Textures)
	{
		tex.Unbind();
	}

	material->Shader.UnBind();
}

eastl::shared_ptr<RenderMaterial> OpenGLRenderer::GetMaterial(const RenderCommand & inCommand) const
{
	switch (DrawMode)
	{
	case EDrawMode::NORMAL:
	{
		return inCommand.Material;
	}
	case EDrawMode::DEPTH:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial("depth_material", materialExists);

		if (!materialExists)
		{
			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/DepthFragmentShader.glsl");
		}

		return depthMaterial;
	}
	case EDrawMode::OUTLINE:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> outlineMaterial = matManager.GetOrAddMaterial("outline_material", materialExists);

		if (!materialExists)
		{
			outlineMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/OutlineFragmentShader.glsl");
		}

		return outlineMaterial;
	}
	}

	return { nullptr };
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

void OpenGLRenderer::AddCommand(const RenderCommand & inCommand)
{
	Commands.push_back(inCommand);
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

