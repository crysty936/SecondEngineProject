#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "EASTL/string.h"


struct SelfRegisteringUniform
{
	SelfRegisteringUniform();
	SelfRegisteringUniform(const uint32_t inValue);
	SelfRegisteringUniform(const float inValue);
	SelfRegisteringUniform(const glm::mat4 inValue);
	SelfRegisteringUniform(const glm::vec3 inValue);
	//SelfRegisteringUniform(const OpenGLTexture& inTexture);

	void Register(const eastl::string& inThisName, const class OpenGLShader& inShader) const;

public:
	enum class UniformType
	{
		Uniform1ui,
		Uniform1f,
		Uniform4fv,
		Uniform3f,
		Texture
	} Type;

	union UniformContainer
	{
		UniformContainer() = default;
		UniformContainer(const uint32_t inValue) : Value1ui{inValue}{}
		UniformContainer(const float inValue) : Value1f{inValue}{}
		UniformContainer(const glm::mat4 inValue) : Value4fv{inValue}{}
		UniformContainer(const glm::vec3 inValue) : Value3f{inValue}{}

		glm::mat4 Value4fv;
		glm::vec3 Value3f;
		float Value1f;
		uint32_t Value1ui;
		//uint32_t TextureHandle;
	}Value;

};