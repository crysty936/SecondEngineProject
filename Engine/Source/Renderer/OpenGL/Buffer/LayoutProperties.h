#pragma once
#include <stdint.h>
#include "Core/EngineUtils.h"
#include "glad/glad.h"

struct LayoutProperties
{
	/** Float or UInt */
	uint32_t Type;
	/** 3 for Basic Vector */
	uint32_t Count;
	uint32_t bNormalized;

	uint32_t GetSizeOfType() const
	{
		switch (Type)
		{
		case GL_FLOAT:			return sizeof(float);
		case GL_UNSIGNED_INT:	return sizeof(unsigned int);
		default:				ASSERT_MSG(0);
		}

		return 0;
	}
};


