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
#include "Entity/Entity.h"
#include "Renderer/Drawable/Drawable.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/Material/RenderMaterial.h"
#include "Buffer/VertexArrayObject.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/Drawable/MirrorQuad.h"
#include "Core/ObjectCreation.h"
#include "OpenGLDepthMap.h"
#include "OpenGLRenderTexture.h"
#include "Renderer/Drawable/DepthMaterial.h"
#include "Core/WindowsPlatform.h"

#include <windows.h>
#include "InputSystem/InputType.h"
#include "Window/WindowsWindow.h"
#include "InputSystem/InputSystem.h"

#if !WITH_GLFW

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

HINSTANCE openglInstance = {};
HDC gldc = {};
using glProc = void(*)();

static void init_opengl_extensions()
{
	// Need methods to create right context, need context to get methods.. so create dummy context

	// Before we can load extensions, we need a dummy OpenGL context, created using a dummy window.
	// We use a dummy window because you can only set the pixel format for a window once. For the
	// real window, we want to use wglChoosePixelFormatARB (so we can potentially specify options
	// that aren't available in PIXELFORMATDESCRIPTOR), but we can't load and use that before we
	// have a context.

	WNDCLASSW windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.hInstance = GetModuleHandleW(0);
	windowClass.lpszClassName = L"Dummy_Window_Class";

	if (!RegisterClassW(&windowClass)) {
		ASSERT_MSG(false, "Failed to register dummy OpenGL window.");
	}

	HWND dummyWindow = CreateWindowExW(
		0,
		windowClass.lpszClassName,
		L"Dummy OpenGL Window",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		windowClass.hInstance,
		0);

	if (!dummyWindow) {
		ASSERT_MSG(false, "Failed to create dummy OpenGL window.");
	}

	ShowWindow(dummyWindow, SW_HIDE);

	MSG msg;
	while (PeekMessageW(&msg, dummyWindow, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	
	HDC dummyDeviceContext = GetDC(dummyWindow);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormat = ChoosePixelFormat(dummyDeviceContext, &pfd);
	if (!pixelFormat) {
		ASSERT_MSG(false, "Failed to find a suitable pixel format.");
	}
	if (!SetPixelFormat(dummyDeviceContext, pixelFormat, &pfd)) {
		ASSERT_MSG(false, "Failed to set the pixel format.");
	}

	HGLRC dummyGLContext = wglCreateContext(dummyDeviceContext);
	if (!dummyGLContext) {
		ASSERT_MSG(false, "Failed to create a dummy OpenGL rendering context.");
	}

	if (!wglMakeCurrent(dummyDeviceContext, dummyGLContext)) {
		ASSERT_MSG(false, "Failed to activate dummy OpenGL rendering context.");
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

	wglMakeCurrent(dummyDeviceContext, 0);
	wglDeleteContext(dummyGLContext);
	ReleaseDC(dummyWindow, dummyDeviceContext);
	DestroyWindow(dummyWindow);
}

static HGLRC init_opengl(HDC real_dc)
{
	init_opengl_extensions();

	// Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
	int pixel_format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB,     true,
		WGL_SUPPORT_OPENGL_ARB,     true,
		WGL_DOUBLE_BUFFER_ARB,      true,
		WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,         32,
		WGL_DEPTH_BITS_ARB,         24,
		WGL_STENCIL_BITS_ARB,       8,
		0
	};

	int pixel_format;
	UINT num_formats;
	wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	if (!num_formats) {
		ASSERT_MSG(false, "Failed to set the OpenGL 3.3 pixel format.");
	}

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
	if (!SetPixelFormat(real_dc, pixel_format, &pfd)) {
		ASSERT_MSG(false, "Failed to set the OpenGL 3.3 pixel format.");
	}

	// Specify that we want to create an OpenGL 3.3 core profile context
	int gl33_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	HGLRC gl33_context = wglCreateContextAttribsARB(real_dc, 0, gl33_attribs);
	if (!gl33_context) {
		ASSERT_MSG(false, "Failed to create OpenGL 3.3 context.");
	}

	if (!wglMakeCurrent(real_dc, gl33_context)) {
		ASSERT_MSG(false, "Failed to activate OpenGL 3.3 rendering context.");
	}

	return gl33_context;
}

static glProc getProcAddressGLWindows(const char* procname)
{
	const glProc proc = (glProc)wglGetProcAddress(procname);
	if (proc)
		return proc;

	return (glProc)GetProcAddress(openglInstance, procname);
}

#endif

const uint32_t SHADOW_WIDTH = 4096;
const uint32_t SHADOW_HEIGHT = 4096;

OpenGLRenderer* OpenGLRenderer::GlobalRHI = nullptr;
static std::mutex RenderCommandsMutex;
static std::mutex LoadQueueMutex;
static std::mutex GetVAOMutex;
static std::condition_variable LoadQueueCondition;

const glm::vec3 lightPos(-10.0f, 10.0f, -1.0f);

constexpr glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);

void LoaderFunc(GLFWwindow* inLoadingThreadContext)
{
	while (Engine->IsRunning())
	{
		eastl::queue<RenderingLoadCommand>& loadQueue = OpenGLRenderer::GetRHI().GetLoadQueue();
		std::unique_lock<std::mutex> lock{ LoadQueueMutex };
		LoadQueueCondition.wait(lock, [&] {return !loadQueue.empty(); });

		RenderingLoadCommand newCommand = loadQueue.front();
		loadQueue.pop();

		lock.unlock();

		glfwMakeContextCurrent(inLoadingThreadContext);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);

		newCommand.LoadDel.Execute(newCommand.ModelPath, newCommand.Parent);

		glfwMakeContextCurrent(nullptr);
	}
}

OpenGLRenderer::OpenGLRenderer(const WindowProperties& inMainWindowProperties)
{
#if !WITH_GLFW
	CurrentWindow = eastl::make_unique<WindowsWindow>();
	openglInstance = LoadLibraryA("opengl32.dll");
	ASSERT(openglInstance);
 	gldc = GetDC(reinterpret_cast<HWND>(CurrentWindow->GetHandle()));
 	HGLRC glrc = init_opengl(gldc);
 
	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)getProcAddressGLWindows) == 1;
	ASSERT(gladSuccess);
	GLWindow = eastl::make_unique<OpenGLWindow>(nullptr, inMainWindowProperties);
#else
	CurrentWindow = eastl::make_unique<WindowsWindow>(false);
	const bool glfwSuccess = glfwInit() == GLFW_TRUE;
  	ASSERT(glfwSuccess);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
  	// Create new Window for data holding
  	GLFWwindow* newWindowHandle = glfwCreateWindow(inMainWindowProperties.Width, inMainWindowProperties.Height, inMainWindowProperties.Title.data(), nullptr, nullptr);
	glfwMakeContextCurrent(newWindowHandle);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;
	ASSERT(gladSuccess);
	glfwSetInputMode(newWindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(newWindowHandle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	GLWindow = eastl::make_unique<OpenGLWindow>(newWindowHandle, inMainWindowProperties);
	InputSystem::Get().RegisterCallbacksGLFW(*GLWindow);
#endif

#if !WITH_GLFW
	InputSystem::Get().SetCursorMode(CurrentWindow->GetHandle(), ECursorMode::Disabled);
#endif
 
	SetViewportSizeToMain();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);

	// Set the default uniforms
	SetupBaseUniforms();

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Create the loading thread
	//std::thread(LoaderFunc, loadingThreadContext).detach();
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Init(const WindowProperties & inMainWindowProperties)
{
	GlobalRHI = new OpenGLRenderer{ inMainWindowProperties };

	// Setup secondary framebuffer

	// Create the frame buffer
	glGenFramebuffers(1, &GlobalRHI->AuxiliarFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, GlobalRHI->AuxiliarFrameBuffer);

	const WindowProperties& windowProps = GlobalRHI->GLWindow->GetProperties();
	// Create a stencil and depth render buffer object for the frame buffer
	uint32_t rbo; // render buffer object
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowProps.Width, windowProps.Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach the rbo to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	// Create the shadow map framebuffer
	glGenFramebuffers(1, &GlobalRHI->ShadowMapBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, GlobalRHI->ShadowMapBuffer);

	GlobalRHI->ShadowBufferTex = eastl::make_shared<OpenGLDepthMap>("ShadowMap");
	GlobalRHI->ShadowBufferTex->Init();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GlobalRHI->ShadowBufferTex->TexHandle, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	// Bind the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::Terminate()
{
	GlobalRHI->GLWindow.reset();

#if WITH_GLFW
	glfwTerminate();
#endif;

	glDeleteBuffers(1, &GlobalRHI->AuxiliarFrameBuffer);

	ASSERT(GlobalRHI);
	delete GlobalRHI;
}

void OpenGLRenderer::Draw()
{
	UpdateUniforms();

	//DrawShadowMap();

	SetupBaseUniforms();
	UpdateUniforms();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//DrawSkybox();
	RenderCommandsMutex.lock();
	DrawCommands(MainCommands);
	RenderCommandsMutex.unlock();

	CheckShouldCloseWindow(*CurrentWindow);


#if !WITH_GLFW
	SwapBuffers(gldc);

#else
	glfwSwapBuffers(GLWindow->GetHandle());
#endif

}

void OpenGLRenderer::DrawSkybox()
{
	if (!MainSkyboxCommand.Parent.lock())
	{
		return;
	}

	if (!MainSkyboxCommand.VAO->bReadyForDraw)
	{
		MainSkyboxCommand.VAO->SetupState();
	}

	glDepthFunc(GL_LEQUAL);
	DrawCommand(MainSkyboxCommand);
	glDepthFunc(GL_LESS);
}

void OpenGLRenderer::DrawShadowMap()
{
	// Cull front face to solve Peter Panning
	//glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SetDrawMode(EDrawMode::DEPTH);
	SetViewportSize(SHADOW_WIDTH, SHADOW_HEIGHT);

	//const glm::vec3 lightLoc = LightSource->GetAbsoluteTransform().Translation;
	const glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const float near_plane = 1.f;
	const float far_plane = 20.f;
	const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	//UniformsCache["projection"] = lightProjection;

	const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	UniformsCache["lightSpaceMatrix"] = lightSpaceMatrix;

	RenderCommandsMutex.lock();
	DrawCommands(MainCommands);
	RenderCommandsMutex.unlock();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SetViewportSizeToMain();
	SetDrawMode(EDrawMode::NORMAL);

	// Reset to default
	//glCullFace(GL_BACK);
}

void OpenGLRenderer::SetupBaseUniforms()
{
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(GLWindow->GetProperties().Width) / static_cast<float>(GLWindow->GetProperties().Height), 0.1f, 1000.0f);
	UniformsCache["projection"] = projection;
}

void OpenGLRenderer::UpdateUniforms()
{
	const glm::mat4 view = SceneManager::Get().GetCurrentScene().CurrentCamera->GetLookAt();
	UniformsCache["view"] = view;
}

void OpenGLRenderer::DrawCommands(const eastl::vector<RenderCommand>&inCommands)
{
	for (const RenderCommand& renderCommand : inCommands)
	{
		DrawCommand(renderCommand);
	}
}

void OpenGLRenderer::DrawCommand(const RenderCommand & inCommand)
{
	const bool parentValid = !inCommand.Parent.expired();
	if (!ENSURE(parentValid))
	{
		return;
	}

	const eastl::shared_ptr<const DrawableObject> parent = inCommand.Parent.lock();
	const eastl::shared_ptr<RenderMaterial> material = GetMaterial(inCommand);
	const eastl::shared_ptr<VertexArrayObject>& vao = inCommand.VAO;

	if (!parent->IsVisible())
	{
		return;
	}

	// TODO: Abstract the model and parent dependent uniforms (like the Model Matrix) to be present in the render command 
	// and updated only if dirty

	// Deffered VAO initialization on the Main Rendering Thread
	if (!vao->bReadyForDraw)
	{
		vao->SetupState();
	}

	material->Shader.Bind();
	material->ResetUniforms();

	UniformsCache["model"] = parent->GetModelMatrix();

	int32_t i = 0;
	for (i = 0; i < material->Textures.size(); ++i)
	{
		eastl::shared_ptr<OpenGLTexture>& tex = material->Textures[i];
		tex->Bind(i);
	}

	//
	ShadowBufferTex->Bind(i);
	//
	GlobalRHI->UniformsCache["ShadowMap"] = uint32_t(i);
	GlobalRHI->UniformsCache["LightPos"] = lightPos;

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

	for (i = 0; i < material->Textures.size(); ++i)
	{
		eastl::shared_ptr<OpenGLTexture>& tex = material->Textures[i];
		tex->Unbind(i);
	}

	//
	ShadowBufferTex->Unbind(i);
	//

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
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial<DepthMaterial>("depth_material", materialExists);

		if (!materialExists)
		{
			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicDepthVertexShader.glsl", "../Data/Shaders/BasicDepthFragmentShader.glsl");
		}

		return depthMaterial;
	}
	case EDrawMode::DEPTH_VISUALIZE:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial("visualize_depth_material", materialExists);

		if (!materialExists)
		{
			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/VisualizeDepthFragmentShader.glsl");
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

void OpenGLRenderer::SetVSyncEnabled(const bool inEnabled)
{
	//glfwSwapInterval(inEnabled);
}

void OpenGLRenderer::AddMirrorCommand(const RenderCommand & inCommand)
{
	// 	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	// 	MirrorCommands.push_back(inCommand);
}

void OpenGLRenderer::AddCommand(const RenderCommand & inCommand)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MainCommands.push_back(inCommand);
}

void OpenGLRenderer::AddCommands(eastl::vector<RenderCommand> inCommands)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MainCommands.insert(MainCommands.end(), inCommands.begin(), inCommands.end());
}

void OpenGLRenderer::AddRenderLoadCommand(const RenderingLoadCommand & inCommand)
{
	std::unique_lock<std::mutex> lock{ LoadQueueMutex };

	LoadQueue.push(inCommand);
	LoadQueueCondition.notify_one();
}

bool OpenGLRenderer::GetOrCreateVAO(const eastl::string & inVAOId, OUT eastl::shared_ptr<VertexArrayObject>&outVAO)
{
	ASSERT(!inVAOId.empty());
	std::lock_guard<std::mutex> uniqueMutex(GetVAOMutex);
	//GetVAOMutex.lock(); // TODO: Why does this not work?

	using iterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<VertexArrayObject>>::iterator;
	const iterator& vaoIter = VAOs.find(inVAOId);
	const bool materialExists = vaoIter != VAOs.end();

	if (materialExists)
	{
		outVAO = (*vaoIter).second;

		return true;
	}

	eastl::shared_ptr<VertexArrayObject> newVAO = eastl::make_shared<VertexArrayObject>();
	VAOs[inVAOId] = newVAO;
	outVAO = newVAO;

	//GetVAOMutex.unlock();

	return false;
}

void OpenGLRenderer::SetViewportSize(const int32_t inWidth, const int32_t inHeight)
{
	glViewport(0, 0, inWidth, inHeight);
}

void OpenGLRenderer::SetViewportSizeToMain()
{
	const WindowProperties& props = GLWindow->GetProperties();
	SetViewportSize(props.Width, props.Height);
}

void OpenGLRenderer::CheckShouldCloseWindow(const WindowsWindow& inWindow)
{
#if !WITH_GLFW
	if(inWindow.ShouldClose())
	{
		StopEngine();
	}
#else
// 	if (glfwWindowShouldClose(inWindow.GetHandle()))
// 	{
// 		StopEngine();
// 	}
#endif

}

