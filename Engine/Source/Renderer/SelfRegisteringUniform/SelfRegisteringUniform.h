#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "EASTL/string.h"
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"

class IUniformData
{
public:
	virtual void* GetData() = 0;
	virtual size_t GetSize() = 0;
};

template<typename T>
class SelfRegisteringUniformData : public IUniformData
{
public:
	SelfRegisteringUniformData(const T& inData)
	{
		Data = inData;
	}

	virtual void* GetData() override
	{
		return &Data;
	}

	virtual size_t GetSize() override 
	{
		return sizeof(T);
	}

public:
	T Data;
};

// Vec3 specialization
template<>
class SelfRegisteringUniformData<glm::vec3> : public IUniformData
{
public:
	SelfRegisteringUniformData(const glm::vec3& inData)
	{
		Data = inData;
	}

	virtual void* GetData() override
	{
		return &Data;
	}

	virtual size_t GetSize() override
	{
		return sizeof(float) * 4; // vec3 has the same alignment as vec4 in GLSL, TODO: Test if this applies in HLSL as well
	}

public:
	glm::vec3 Data;
};

template<typename T>
class SelfRegisteringUniformData<eastl::vector<T>> : public IUniformData
{
public:
	SelfRegisteringUniformData(const eastl::vector<T>& inData)
	{
		Data = inData;
	}

	virtual void* GetData() override
	{
		return Data.data();
	}

	virtual size_t GetSize() override
	{
		return sizeof(T) * Data.size();
	}

public:
	eastl::vector<T> Data;
};

struct SelfRegisteringUniform
{
	SelfRegisteringUniform();
	SelfRegisteringUniform(const uint32_t inValue);
	SelfRegisteringUniform(const float inValue);
	SelfRegisteringUniform(const glm::mat4 inValue);
	SelfRegisteringUniform(const glm::vec3 inValue);
	SelfRegisteringUniform(const eastl::vector<glm::mat4>& inValue);

	void Register(class UniformBufferContainer& inBuffer) const;

public:
	enum class UniformType
	{
		Uniform1ui,
		Uniform1f,
		Uniform4fv,
		Uniform3f,
		Uniform4fvArray
	} Type;

	template<typename T>
	T& GetValue() 
	{
		SelfRegisteringUniformData<T>* typedUniform = static_cast<SelfRegisteringUniformData<T>*>(Data.get());
		return typedUniform->Data;
	}

	eastl::unique_ptr<IUniformData> Data;
};