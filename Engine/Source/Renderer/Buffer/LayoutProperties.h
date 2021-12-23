#pragma once
#include <stdint.h>
#include "Core/EngineUtils.h"
#include "glad/glad.h"

struct LayoutProperties
{
	uint32_t Type;
	uint32_t Count;
	uint32_t bNormalized;

	uint32_t GetSizeOfType()
	{
		switch (Type)
		{
		case GL_FLOAT:			return sizeof(float);
		case GL_UNSIGNED_INT:	return sizeof(unsigned int);
		default:				ASSERT(0);
		}

		return 0;
	}
};


