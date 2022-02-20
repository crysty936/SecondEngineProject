#pragma once
#include "EASTL/string.h"

// TODO: Template these

class SelfRegisteringUniformBase
{
public:
	SelfRegisteringUniformBase();
	virtual ~SelfRegisteringUniformBase();

	virtual void RegisterUniform(const class OpenGLShader& inShader, const eastl::string& inName) = 0;
};