#pragma once
#include "glm/common.hpp"
#include "glm/ext/vector_float3.hpp"
#include "Core/EngineUtils.h"
#include "EASTL/array.h"

struct AABB
{
	glm::vec3 Min;
	glm::vec3 Max;

	inline AABB& operator +=(const glm::vec3& inVec)
	{
		if (IsInitialized)
		{
			Min.x = glm::min(Min.x, inVec.x);
			Min.y = glm::min(Min.y, inVec.y);
			Min.z = glm::min(Min.z, inVec.z);

			Max.x = glm::max(Max.x, inVec.x);
			Max.y = glm::max(Max.y, inVec.y);
			Max.z = glm::max(Max.z, inVec.z);
		}
		else
		{
			Min = Max = inVec;
			IsInitialized = true;
		}

		return *this;
	}

	inline glm::vec3 GetExtent() const
	{
		return 0.5f * (Max - Min);
	}

	inline void GetCenterAndExtent(OUT glm::vec3& outCenter, OUT glm::vec3& outExtent) const
	{
		outExtent = GetExtent();
		outCenter = Min + outExtent;
	}

	eastl::array<glm::vec3, 8> GetVertices();

private:
	bool IsInitialized = false;
};