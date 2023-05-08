#include <assert.h>
#include "Renderer/Renderer.h"
#include "RHI/RHI.h"
#include "ForwardRenderer.h"
#include "DeferredRenderer.h"
#include "Window/WindowsWindow.h"
#include "Core/EngineCore.h"

#define RENDERER_FORWARD 1
#define RENDERER_DEFERRED 2
#define CHOSEN_RENDERER 1

constexpr glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);

Renderer::Renderer(const WindowProperties& inMainWindowProperties)
{
	SetViewportSizeToMain();

	RHI::Get()->ClearColor(ClearColor);

	// Set the default uniforms
	SetBaseUniforms();
}

Renderer::~Renderer() = default;

void Renderer::Init(const WindowProperties& inMainWindowProperties)
{
#if CHOSEN_RENDERER == RENDERER_FORWARD
	Instance = new ForwardRenderer{ inMainWindowProperties };
#elif CHOSEN_RENDERER == RENDERER_DEFERRED
	Instance = new DeferredRenderer{ inMainWindowProperties };
#endif

	Instance->InitInternal();
}

void Renderer::Terminate()
{
	ASSERT(Instance);
	delete Instance;
}

void Renderer::SetViewportSizeToMain()
{
	const WindowsWindow& currentWindow = Engine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();
	RHI::Get()->SetViewportSize(props.Width, props.Height);
}

void Renderer::SetBaseUniforms()
{
	// By default, use a D3D11 projection matrix.
	// Note: glm is RH but uses a sneaky minus to change the handedness of the output to LH (how OpenGL actually is)
	const float windowWidth = static_cast<float>(Engine->GetMainWindow().GetProperties().Width);
	const float windowHeight = static_cast<float>(Engine->GetMainWindow().GetProperties().Height);

	glm::mat4 projection = glm::perspectiveRH_ZO(glm::radians(CAMERA_FOV), windowWidth / windowHeight, CAMERA_NEAR, CAMERA_FAR);
	//glm::mat4 lightProjection = glm::orthoRH_ZO(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	RHI::Get()->PrepareProjectionForRendering(projection);

	UniformsCache["projection"] = projection;
}

