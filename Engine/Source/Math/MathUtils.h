#pragma once
#include "glm/common.hpp"


namespace MathUtils
{
	inline int GetRandom(int32_t inMinValue, int32_t inMaxValue) { return glm::max(inMinValue, (rand() % inMaxValue)); }
}