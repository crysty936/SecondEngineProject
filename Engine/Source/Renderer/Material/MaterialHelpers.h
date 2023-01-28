#pragma once
#include "EASTL/vector.h"
#include "EASTL/string.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/RHI/Resources/UniformBufferContainer.h"
#include "EASTL/unordered_map.h"
#include "Renderer/RHI/Resources/RHIUniformBuffer.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"

class MaterialHelpers
{
public:
	template<typename T>
	static bool TryGetUniformFromCache(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache, const eastl::string& inUniformName, T& outUniform);

private:
	template<typename T>
	static T& GetUniformValue(SelfRegisteringUniform& inUniform);
};

template<typename T>
bool MaterialHelpers::TryGetUniformFromCache(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache, const eastl::string& inUniformName, T& outUniform)
{
	auto& iter = inUniformsCache.find(inUniformName);
	 
	if (iter == inUniformsCache.end())
	{
	 	return false;
	}
	 
	outUniform = GetUniformValue<T>((*iter).second);

	return true;
}

template<typename T>
T& MaterialHelpers::GetUniformValue(SelfRegisteringUniform& inUniform)
{
	static_assert(false, "GetUniformValue called with unsupported value");
}

template<>
inline glm::mat4& MaterialHelpers::GetUniformValue<glm::mat4>(SelfRegisteringUniform& inUniform)
{
	return inUniform.Value.Value4fv;
}

template<>
inline glm::vec3& MaterialHelpers::GetUniformValue<glm::vec3>(SelfRegisteringUniform& inUniform)
{
	return inUniform.Value.Value3f;
}

template<>
inline float& MaterialHelpers::GetUniformValue<float>(SelfRegisteringUniform& inUniform)
{
	return inUniform.Value.Value1f;
}

template<>
inline uint32_t& MaterialHelpers::GetUniformValue<uint32_t>(SelfRegisteringUniform& inUniform)
{
	return inUniform.Value.Value1ui;
}
