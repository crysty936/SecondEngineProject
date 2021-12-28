#include <stdint.h>
#include <assert.h>
#include "Logger/Logger.h"
#include "Core/EngineUtils.h"
#include "glad/glad.h"


namespace OpenGLUtils
{
	void GLFWErrorCallback(int32_t inErrorType, const char* inErrorDesc)
	{
		LOG_ERROR(inErrorDesc);

		ASSERT(0, "OpenGl Error.");
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
}


