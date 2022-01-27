#pragma once
#include "EASTL/unordered_map.h"
#include "EASTL/string.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glad/glad.h"

class OpenGLShader
{
public:
	static OpenGLShader ConstructShaderFromSource(const eastl::string& inVertexSource, const eastl::string& inFragmentSource);
	static OpenGLShader ConstructShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inFragmentPath);

public:
	void Bind() const;
	static void UnBind();
	void SetUniformValue4f(const eastl::string& UniformName, float v1, float v2, float v3, float v4) const;
	void SetUniformValue3f(const eastl::string& UniformName, float v1, float v2, float v3) const;
	void SetUniformValue1f(const eastl::string& UniformName, float v1) const;
	void SetUniformValue1i(const eastl::string& UniformName, int v1) const;
	void SetUniformValue4fv(const eastl::string& UniformName, glm::mat4 matrix) const;
	void SetUniformValue3fv(const eastl::string& UniformName, glm::vec3 vec) const;

	uint32_t inline GetHandle() const { return ShaderHandle; }
	void DeleteShader();

public:
	OpenGLShader(const eastl::string& inVertexSrc, const eastl::string& inFragmentSrc);
	OpenGLShader() = default;
	~OpenGLShader();
	OpenGLShader(const OpenGLShader& inOther) = default;
	OpenGLShader& operator=(const OpenGLShader& inOther) = default;
	OpenGLShader(OpenGLShader&& inOther) = default;
	OpenGLShader& operator=(OpenGLShader&& inOther) = default;

private:
	int GetUniformLocation(const eastl::string& UniformName) const;
	uint32_t CreateShader(const eastl::string& Source, GLenum ShaderType);

private:
	eastl::unordered_map<eastl::string, int> UniformLocations;
	uint32_t ShaderHandle = 0;
};
