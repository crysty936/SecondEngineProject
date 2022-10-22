#include "OpenGLLayoutProperties.h"
#include "glad/glad.h"

uint32_t OpenGLLayoutProperties::GetSizeOfType() const
{
	switch (Type)
	{
	case GL_FLOAT:			return sizeof(float);
	case GL_UNSIGNED_INT:	return sizeof(unsigned int);
	default:				ASSERT(0);
	}

	return 0;
}
