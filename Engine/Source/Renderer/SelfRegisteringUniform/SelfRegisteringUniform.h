#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "EASTL/string.h"


struct SelfRegisteringUniform
{
	SelfRegisteringUniform();
	SelfRegisteringUniform(const float inValue);
	SelfRegisteringUniform(const glm::mat4 inValue);
	//SelfRegisteringUniform(const OpenGLTexture& inTexture);

	void Register(const eastl::string& inThisName, const class OpenGLShader& inShader) const;

private:
	enum class UniformType
	{
		Uniform1f,
		Uniform4fv,
		Texture
	} Type;

	union UniformContainer
	{
		UniformContainer() = default;
		UniformContainer(const float inValue) : Value1f{inValue}{}
		UniformContainer(const glm::mat4 inValue) : Value4fv{inValue}{}
		//UniformContainer(const uint32_t inTextureHandle) : TextureHandle{inTextureHandle}{}

		float Value1f;
		glm::mat4 Value4fv;
		//uint32_t TextureHandle;
	}Value;

};