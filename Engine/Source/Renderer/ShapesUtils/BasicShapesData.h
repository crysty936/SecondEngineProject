#pragma once

/**
 * File containing basic shapes vertices and indices
 */

namespace BasicShapes
{
	// Triangle
	const float TriangleVertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	const uint32_t TriangleIndices[] =
	{
		0, 1, 2
	};
	// Triangle

	// Square
	float SquareVertices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	uint32_t SquareIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	// Square
}
