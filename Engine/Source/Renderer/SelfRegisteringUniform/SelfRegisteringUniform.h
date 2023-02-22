#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "EASTL/string.h"
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/RHI/Resources/UniformBufferContainer.h"

class IUniformData
{
public:
	virtual void SelfRegister(UniformBufferContainer& inBufferContainer) = 0;

private:
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


	virtual void SelfRegister(UniformBufferContainer& inBufferContainer) override
	{
		inBufferContainer.AddData(GetData(), GetSize());
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
		AlignedData = glm::vec4(Data.x, Data.y, Data.z, 1.0);
		return &AlignedData;
	}

	virtual size_t GetSize() override
	{
		return sizeof(float) * 4; // Align to 16 bytes
	}

	virtual void SelfRegister(UniformBufferContainer& inBufferContainer) override
	{
		inBufferContainer.AddData(GetData(), GetSize());
	}

public:
	glm::vec3 Data;
	glm::vec4 AlignedData;
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

	virtual void SelfRegister(UniformBufferContainer& inBufferContainer) override
	{
		// Each element in an array has a base aligment of 4 * 4 = 16 bytes ( vec4 )

		if (sizeof(T) < 16)
		{
			// Each element is smaller than the required number, add one by one and let the buffer container align them
			for (const T& Element : Data)
			{
 				//char newData[16];
 				//memcpy(&newData[16 - sizeof(T)], &Element, sizeof(T));
				//inBufferContainer.AddData(&newData, 16, false);

				inBufferContainer.AddData(&Element, sizeof(T), true);
			}

			return;
		}

		// Elements have the right alignment, add all of them at once
		inBufferContainer.AddData(GetData(), GetSize());
	}

public:
	eastl::vector<T> Data;
};

struct SelfRegisteringUniform
{
	SelfRegisteringUniform();
	SelfRegisteringUniform(const int32_t inValue);
	SelfRegisteringUniform(const uint32_t inValue);
	SelfRegisteringUniform(const float inValue);
	SelfRegisteringUniform(const glm::mat4 inValue);
	SelfRegisteringUniform(const glm::vec3 inValue);
	SelfRegisteringUniform(const glm::vec4 inValue);
	SelfRegisteringUniform(const eastl::vector<float>& inValue);
	SelfRegisteringUniform(const eastl::vector<glm::mat4>& inValue);

	void Register(class UniformBufferContainer& inBuffer) const;

public:
	enum class UniformType
	{
		Uniform1i,
		Uniform1ui,
		Uniform1f,
		Uniform4fv,
		Uniform3f,
		Uniform4f,
		UniformfArray,
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