#pragma once
#include <stdint.h>

/**
 * Container for basic shapes data
 */

namespace BasicShapesData
{
	const float* GetTriangleVertices();
	const uint32_t* GetTriangleIndices();
	int32_t GetTriangleVerticesCount();
	int32_t GetTriangleIndicesCount();

	const float* GetSquareVertices();
	const uint32_t* GetSquareIndices();
	int32_t GetSquareVerticesCount();
	int32_t GetSquareIndicesCount();
}
