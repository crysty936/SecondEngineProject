#include <stdint.h>
#include <assert.h>
#include "Logger/Logger.h"
#include "Core/EngineUtils.h"
#include "glad/glad.h"


namespace OpenGLUtils
{
	void GLFWErrorCallback(int32_t inErrorType, const char* inErrorDesc)
	{
		LOG(inErrorDesc);

		ASSERT(0, "OpenGl Error.");
	}

	void GLDebugCallback(GLenum inSource, GLenum inType, GLenum inId, GLenum inSeverity, GLsizei inLength, const GLchar* inMessage, const void* userParam)
	{
		if (inSeverity != GL_DEBUG_SEVERITY_HIGH)
		{
			LOG("OpenGL Error: %s", inMessage);
		}
		else
		{
			LOG("OpenGL Critical Error: %s", inMessage);
			ASSERT(0);
		}
	}
}


