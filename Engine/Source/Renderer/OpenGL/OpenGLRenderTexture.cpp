#include "OpenGLRenderTexture.h"
#include "stb_image.h"
#include "Utils/ImageLoading.h"
#include "Window/WindowProperties.h"
#include "Window/WindowsWindow.h"
#include "glad/glad.h"
#include "Core/EngineCore.h"

OpenGLRenderTexture::OpenGLRenderTexture(const eastl::string& inTexName)
	/*: OpenGLTexture(inTexName, GL_TEXTURE_2D)*/ {}
OpenGLRenderTexture::~OpenGLRenderTexture() = default;

void OpenGLRenderTexture::Init()
{
// 	glGenTextures(1, &TexHandle);
// 	glBindTexture(GLTexType, TexHandle);
// 
// 	const WindowProperties& windowProps = Engine->GetMainWindow().GetProperties();
// 
// 	glTexImage2D(GLTexType, 0, GL_RGB, windowProps.Width, windowProps.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
// 
// 	glTexParameteri(GLTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GLTexType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 
// 	glTexParameteri(GLTexType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
// 	glTexParameteri(GLTexType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
// 
// 	glBindTexture(GLTexType, 0);
}

