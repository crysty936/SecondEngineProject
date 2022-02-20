#pragma once
#include "SelfRegisteringUniformBase.h"
#include "EASTL/string.h"
#include "glm/ext/matrix_float4x4.hpp"

class SelfRegisteringUniform4fv : public SelfRegisteringUniformBase
{
public:
	SelfRegisteringUniform4fv(const glm::mat4 inValue);
	virtual ~SelfRegisteringUniform4fv();

	virtual void RegisterUniform(const class OpenGLShader& inShader, const eastl::string& inName) override;

private:
	glm::mat4 Value;
};