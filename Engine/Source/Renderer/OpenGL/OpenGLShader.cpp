#include "Renderer/OpenGL/OpenGLShader.h"
#include "EASTL/string.h"
#include "Utils/IOUtils.h"
#include <glm/gtc/type_ptr.hpp>
#include "Core/EngineUtils.h"

OpenGLShader OpenGLShader::ConstructShaderFromSource(const eastl::string& inVertexSource, const eastl::string& inFragmentSource)
{
	return OpenGLShader{ inVertexSource, inFragmentSource };
}

OpenGLShader OpenGLShader::ConstructShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inFragmentPath)
{
	eastl::string vertexShaderCode;
	eastl::string fragmentShaderCode;

	const bool vertexReadSuccess = TryFastReadFile(inVertexPath, vertexShaderCode);
	const bool fragmentReadSuccess = TryFastReadFile(inFragmentPath, fragmentShaderCode);

	return ConstructShaderFromSource(vertexShaderCode, fragmentShaderCode);
}

void OpenGLShader::DeleteShader()
{
	if (ShaderHandle != 0)
	{
		glDeleteProgram(ShaderHandle);
	}
}

OpenGLShader::OpenGLShader(const eastl::string& inVertexSrc, const eastl::string& inFragmentSrc)
{
	// Create an empty vertex shader handle
	GLuint vertexShader = CreateShader(inVertexSrc, GL_VERTEX_SHADER);

	// Create an empty fragment shader handle
	GLuint fragmentShader = CreateShader(inFragmentSrc, GL_FRAGMENT_SHADER);

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	GLuint program = glCreateProgram();
	ShaderHandle = program;

	// Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		char* infoLog = (char*)_malloca(maxLength * sizeof(char));
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		LOG_ERROR("%s", infoLog);
		ASSERT_MSG(0, "Shader Link failure.");

		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
}

OpenGLShader::~OpenGLShader() = default;

void OpenGLShader::Bind() const
{
	glUseProgram(ShaderHandle);
}

void OpenGLShader::UnBind()
{
	glUseProgram(0);
}

void OpenGLShader::SetUniformValue4f(const eastl::string & UniformName, float v1, float v2, float v3, float v4) const
{
	glUniform4f(GetUniformLocation(UniformName), v1, v2, v3, v4);
}
void OpenGLShader::SetUniformValue3f(const eastl::string & UniformName, float v1, float v2, float v3) const
{
	glUniform3f(GetUniformLocation(UniformName), v1, v2, v3);
}

void OpenGLShader::SetUniformValue1f(const eastl::string & UniformName, float v1) const
{
	glUniform1f(GetUniformLocation(UniformName), v1);
}

void OpenGLShader::SetUniformValue1i(const eastl::string & UniformName, int v1) const
{
	glUniform1i(GetUniformLocation(UniformName), v1);
}

void OpenGLShader::SetUniformValue4fv(const eastl::string & UniformName, glm::mat4 matrix) const
{
	glUniformMatrix4fv(GetUniformLocation(UniformName), 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::SetUniformValue3fv(const eastl::string & UniformName, glm::vec3 vec) const
{
	glUniform3fv(GetUniformLocation(UniformName), 1, glm::value_ptr(vec));
}

int OpenGLShader::GetUniformLocation(const eastl::string & UniformName) const
{
	if (UniformLocations.find(UniformName) != UniformLocations.end())
		return UniformLocations[UniformName];

	int uniformLocation = glGetUniformLocation(GetHandle(), UniformName.c_str());
	if (uniformLocation != -1)
		UniformLocations[UniformName] = uniformLocation;

	ASSERT_MSG(uniformLocation != -1, "Uniform location could not be found!");

	return uniformLocation;
}

uint32_t OpenGLShader::CreateShader(const eastl::string & Source, GLenum ShaderType)
{
	GLuint shaderHandle = glCreateShader(ShaderType);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = Source.c_str();
	glShaderSource(shaderHandle, 1, &source, nullptr);

	// Compile the vertex shader
	glCompileShader(shaderHandle);

	GLint isCompiled = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		int bytesWritten;
		char* infoLog = (char*)_malloca(maxLength * sizeof(char));
		glGetShaderInfoLog(shaderHandle, maxLength, &bytesWritten, infoLog);

		// We don't need the shader anymore.
		glDeleteShader(shaderHandle);

		LOG_ERROR("%s", infoLog);
		ASSERT_MSG(0, "Shader Compilation failure.");
		return -1;
	}

	return shaderHandle;
}