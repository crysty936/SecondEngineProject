#include <stdint.h>
#include <assert.h>
#include "Logger/Logger.h"

namespace GLUtils
{
	void GLFWErrorCallback(int32_t inErrorType, const char* inErrorDesc)
	{
		Logger::Get().Print(inErrorDesc);

		assert(0);



	}
}


