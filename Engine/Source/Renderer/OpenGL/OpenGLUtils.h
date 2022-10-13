#pragma once

namespace OpenGLUtils
{
	void GLFWErrorCallback(int32_t inErrorType, const char* inErrorDesc);
	void GLDebugCallback(uint32_t inSource, uint32_t inType, uint32_t inId, uint32_t inSeverity, int32_t inLength, const char* inMessage, const void* userParam);
}