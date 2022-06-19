#pragma once
#include <stdint.h>

/**
 * Container for basic shapes data
 */

namespace BasicShapesData
{
	// Triangle
	const float* GetTriangleVertices();
	const uint32_t* GetTriangleIndices();
	int32_t GetTriangleVerticesCount();
	int32_t GetTriangleIndicesCount();

	// Square
	const float* GetSquareVertices();
	const uint32_t* GetSquareIndices();
	int32_t GetSquareVerticesCount();
	int32_t GetSquareIndicesCount();

	// Quad
	const float* GetQuadVertices();
	const uint32_t* GetQuadIndices();
	int32_t GetQuadVerticesCount();
	int32_t GetQuadIndicesCount();

	// Cube
	const float* GetCubeVertices();
	const uint32_t* GetCubeIndices();
	int32_t GetCubeVerticesCount();
	int32_t GetCubeIndicesCount();

	// Skybox
	const float* GetSkyboxVertices();
	int32_t GetSkyboxVerticesCount();
	int32_t GetSkyboxIndicesCount();
}
