#include "OpenGLRHI.h"
#include <windows.h>

#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"
#include "Window/WindowsWindow.h"
#include "Renderer/RHI/OpenGL/Resources/GLIndexBuffer.h"
#include "Renderer/RHI/OpenGL/Resources/GLShader.h"
#include "Utils/IOUtils.h"
#include "Renderer/RHI/OpenGL/Resources/GLUniformBuffer.h"
#include "Renderer/RHI/OpenGL/Resources/GLVertexBuffer.h"
#include "Utils/ImageLoading.h"

#include "glad/glad.h"
#include "Renderer/RHI/OpenGL/Resources/GLTexture2D.h"
#include "glm/ext/matrix_transform.hpp"

namespace GLUtils
{

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

	void GLDebugCallback(GLenum inSource, GLenum inType, GLenum inId, GLenum inSeverity, GLsizei inLength, const GLchar* inMessage, const void* userParam)
	{
		switch (inSeverity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
		{
			LOG_ERROR("OpenGL Critical Error: %s", inMessage);
			ASSERT(0);
			break;
		}

		case GL_DEBUG_SEVERITY_LOW:
		{
			LOG_WARNING("OpenGL Severity Low: %s", inMessage);
			break;
		}

		case GL_DEBUG_SEVERITY_MEDIUM:
		{
			LOG_WARNING("OpenGL Severity Medium: %s", inMessage);

			break;
		}
		case GL_DEBUG_SEVERITY_NOTIFICATION:
		{
			LOG_INFO("OpenGL Info: %s", inMessage);

			break;
		}
		}
	}

	static HGLRC init_opengl(HDC real_dc)
	{
		GLUtils::init_opengl_extensions();

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
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 2,
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

		return (glProc)GetProcAddress(GLUtils::openglInstance, procname);
	}

	uint32_t GlobalVAOHandle = 0;
}

OpenGLRHI::OpenGLRHI()
{
	GLUtils::openglInstance = LoadLibraryA("opengl32.dll");
	ASSERT(GLUtils::openglInstance);
	GLUtils::gldc = GetDC(static_cast<HWND>(Engine->GetMainWindow().GetHandle()));
	HGLRC glrc = GLUtils::init_opengl(GLUtils::gldc);

	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)GLUtils::getProcAddressGLWindows) == 1;
	ASSERT(gladSuccess);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);

	glDisable(GL_DEPTH_CLAMP);

	glDebugMessageCallback(GLUtils::GLDebugCallback, nullptr);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glGenVertexArrays(1, &GLUtils::GlobalVAOHandle);
	glBindVertexArray(GLUtils::GlobalVAOHandle);
}

OpenGLRHI::~OpenGLRHI() = default;

eastl::shared_ptr<RHIVertexBuffer> OpenGLRHI::CreateVertexBuffer(const VertexInputLayout& inLayout, const float* inVertices, const int32_t inCount, eastl::shared_ptr<RHIIndexBuffer> inIndexBuffer)
{
	uint32_t handle = 0;
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);

	glNamedBufferData(handle, sizeof(float) * inCount, inVertices, GL_STATIC_DRAW);

	eastl::shared_ptr<GLVertexBuffer> newBuffer = eastl::make_shared<GLVertexBuffer>(handle, inIndexBuffer, inLayout);
	return newBuffer;
}

eastl::shared_ptr<RHIVertexBuffer> OpenGLRHI::CreateVertexBuffer(const VertexInputLayout& inLayout, const eastl::vector<Vertex>& inVertices, eastl::shared_ptr<RHIIndexBuffer> inIndexBuffer)
{
	uint32_t handle = 0;
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);

	const int32_t verticesCount = static_cast<int32_t>(inVertices.size());
	glNamedBufferData(handle, sizeof(Vertex) * verticesCount, inVertices.data(), GL_STATIC_DRAW);

	eastl::shared_ptr<GLVertexBuffer> newBuffer = eastl::make_shared<GLVertexBuffer>(handle, inIndexBuffer, inLayout);
	return newBuffer;
}

eastl::shared_ptr<RHIIndexBuffer> OpenGLRHI::CreateIndexBuffer(const uint32_t* inData, uint32_t inCount)
{
	uint32_t handle = 0;
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);

	glNamedBufferData(handle, sizeof(uint32_t) * inCount, inData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	eastl::shared_ptr<GLIndexBuffer> buffer = eastl::make_shared<GLIndexBuffer>(handle, inCount);

	return buffer;
}

eastl::shared_ptr<RHIUniformBuffer> OpenGLRHI::CreateUniformBuffer(size_t inSize)
{
	uint32_t bufferHandle = 0;
	glGenBuffers(1, &bufferHandle);
	ASSERT(bufferHandle != -1);

	glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
	glBufferData(GL_UNIFORM_BUFFER, inSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, bufferHandle, 0, inSize);

	eastl::shared_ptr<GlUniformBuffer> newBuffer = eastl::make_shared<GlUniformBuffer>(bufferHandle, inSize);

	return newBuffer;
}

eastl::shared_ptr<RHITexture2D> OpenGLRHI::CreateTexture2D()
{
	uint32_t texHandle = 0;
	glGenTextures(1, &texHandle);

	eastl::shared_ptr<GLTexture2D> newTexture = eastl::make_shared<GLTexture2D>(texHandle);

	return newTexture;
}

void OpenGLRHI::UniformBufferUpdateData(RHIUniformBuffer& inBuffer, const void* inData, const size_t inDataSize)
{
	const GlUniformBuffer& glBuffer = static_cast<const GlUniformBuffer&>(inBuffer);
	ASSERT(inDataSize == glBuffer.InitSize);
	
	glBindBuffer(GL_UNIFORM_BUFFER, glBuffer.GLHandle);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, inDataSize, inData);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLRHI::LoadTextureFromPath(RHITexture2D& inTexture, const eastl::string& inPath)
{
	GLTexture2D& tex = static_cast<GLTexture2D&>(inTexture);
	ImageData data = ImageLoading::LoadImageData(inPath.data());

	if (!data.RawData)
	{
		return;
	}

	tex.NrChannels = data.NrChannels;

	glBindTexture(GL_TEXTURE_2D, tex.GlHandle);

	GLenum imageFormat = 0;
	switch (data.NrChannels)
	{
	case 1:
	{
		imageFormat = GL_RED;
		break;
	}
	case 3:
	{
		imageFormat = GL_RGB;
		break;
	}
	case 4:
	{
		imageFormat = GL_RGBA;
		break;
	}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, data.Width, data.Height, 0, imageFormat, GL_UNSIGNED_BYTE, data.RawData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	ImageLoading::FreeImageData(data.RawData);
}

void OpenGLRHI::SetViewportSize(const int32_t inWidth, const int32_t inHeight)
{
	glViewport(0, 0, inWidth, inHeight);
}

void OpenGLRHI::ClearColor(const glm::vec4 inColor)
{
	// Set the clear color to be used when cleaning the back buffers
	glClearColor(inColor.x, inColor.y, inColor.z, inColor.w);
}

void OpenGLRHI::SwapBuffers()
{
	::SwapBuffers(GLUtils::gldc);
}

void OpenGLRHI::ClearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRHI::PrepareProjectionForRendering(glm::mat4& inProj)
{
	inProj[2][2] = (inProj[2][2] * 2.f) - (-1.f);
	inProj[3][2] *= 2.f;
}

void OpenGLRHI::BindVertexBuffer(const RHIVertexBuffer& inBuffer, const bool inBindIndexBuffer)
{
	const GLVertexBuffer& glBuffer = static_cast<const GLVertexBuffer&>(inBuffer);
	if (inBindIndexBuffer)
	{
		BindIndexBuffer(*(glBuffer.IndexBuffer));
	}

	glBindBuffer(GL_ARRAY_BUFFER, glBuffer.Handle);

	// Appy buffer layout
	const eastl::vector<VertexLayoutProperties>& props = glBuffer.Layout.GetProperties();

	size_t offset = 0;
	for (int32_t i = 0; i < props.size(); i++)
	{
		const VertexLayoutProperties& prop = props[i];

		void* offsetPtr = reinterpret_cast<void*>(offset);
		uint32_t glType = 0;

		switch (prop.Type)
		{
		case VertexPropertyType::UInt:
			glType = GL_UNSIGNED_INT;
			break;
		case VertexPropertyType::Float:
			glType = GL_FLOAT;
			break;
		}

		glVertexAttribPointer(i, prop.Count, glType, prop.bNormalized, glBuffer.Layout.GetStride(), offsetPtr);
		glEnableVertexAttribArray(i);

		offset += prop.Count * prop.GetSizeOfType();
	}
}

void OpenGLRHI::BindIndexBuffer(const RHIIndexBuffer& inBuffer)
{
	const GLIndexBuffer& glBuffer = static_cast<const GLIndexBuffer&>(inBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBuffer.Handle);
}

void OpenGLRHI::BindShader(const RHIShader& inShader)
{
	const GLShader& glShader = static_cast<const GLShader&>(inShader);
	glUseProgram(glShader.Handle);
}

void OpenGLRHI::BindUniformBuffer(const RHIUniformBuffer& inBuffer)
{
	const GlUniformBuffer& glbuffer = static_cast<const GlUniformBuffer&>(inBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, glbuffer.GLHandle);
}

void OpenGLRHI::UnbindVertexBuffer(const RHIVertexBuffer& inBuffer, const bool inUnbindIndexBuffer)
{
	if (inUnbindIndexBuffer)
	{
		UnbindIndexBuffer(*(inBuffer.IndexBuffer));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLRHI::UnbindIndexBuffer(const RHIIndexBuffer& inBuffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void OpenGLRHI::UnbindShader(const RHIShader& inShader)
{
	glUseProgram(0);
}

void OpenGLRHI::UnbindUniformBuffer(const RHIUniformBuffer& inBuffer)
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

uint32_t CreateShaderInternal(const eastl::string& Source, uint32_t ShaderType)
{
	GLuint shaderHandle = glCreateShader(ShaderType);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = Source.c_str();
	glShaderSource(shaderHandle, 1, &source, nullptr);

	// Compile the vertex shader
	glCompileShader(shaderHandle);

	GLint isCompiled = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		int bytesWritten;
		char* infoLog = (char*)_malloca(maxLength * sizeof(char));
		glGetShaderInfoLog(shaderHandle, maxLength, &bytesWritten, infoLog);

		// We don't need the shader anymore.
		glDeleteShader(shaderHandle);

		LOG_ERROR("%s", infoLog);
		ASSERT_MSG(0, "Shader Compilation failure.");
		return -1;
	}

	return shaderHandle;
}

eastl::shared_ptr<class RHIShader> OpenGLRHI::CreateShaderFromSource(const eastl::string& inVertexSrc, const eastl::string& inPixelSrc, const VertexInputLayout& inInputLayout)
{

	// Create an empty vertex shader handle
	GLuint vertexShader = CreateShaderInternal(inVertexSrc, GL_VERTEX_SHADER);

	// Create an empty fragment shader handle
	GLuint fragmentShader = CreateShaderInternal(inPixelSrc, GL_FRAGMENT_SHADER);

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	GLuint program = glCreateProgram();

	// Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* infoLog = (char*)_malloca(maxLength * sizeof(char));
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		LOG_ERROR("%s", infoLog);
		ASSERT_MSG(0, "Shader Link failure.");

		return nullptr;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	eastl::shared_ptr<GLShader> newShader = eastl::make_shared<GLShader>(program);
	return newShader;
}

eastl::shared_ptr<class RHIShader> OpenGLRHI::CreateShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inPixelPath, const VertexInputLayout& inInputLayout)
{
	eastl::string fullVertexpath = inVertexPath;
	fullVertexpath.insert(0, "../Data/Shaders/OpenGL/");
	fullVertexpath.append(".glsl");

	eastl::string fullPixelPath = inPixelPath;
	fullPixelPath.insert(0, "../Data/Shaders/OpenGL/");
	fullPixelPath.append(".glsl");

	eastl::string vertexShaderCode;
	eastl::string fragmentShaderCode;

	const bool vertexReadSuccess = IOUtils::TryFastReadFile(fullVertexpath, vertexShaderCode);
	const bool fragmentReadSuccess = IOUtils::TryFastReadFile(fullPixelPath, fragmentShaderCode);

	return CreateShaderFromSource(vertexShaderCode, fragmentShaderCode, inInputLayout);
}

void OpenGLRHI::DrawElements(const int32_t inElementsCount)
{
	glDrawElements(GL_TRIANGLES, inElementsCount, GL_UNSIGNED_INT, 0);
}