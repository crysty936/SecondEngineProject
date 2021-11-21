#include <stdint.h>
#include <assert.h>
#include "Logger/Logger.h"
#include "Core/EngineUtils.h"

namespace GLUtils
{
	void GLFWErrorCallback(int32_t inErrorType, const char* inErrorDesc)
	{
		Logger::Get().Print(inErrorDesc);

		ASSERT(0, "OpenGl Error.");
	}
}


