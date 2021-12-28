#pragma once


namespace OpenGLUtils
{
	void GLFWErrorCallback(int32_t inErrorType, const char* inErrorDesc);
	void GLDebugCallback(GLenum inSource, GLenum inType, GLenum inId, GLenum inSeverity, GLsizei inLength, const GLchar* inMessage, const void* userParam);
}