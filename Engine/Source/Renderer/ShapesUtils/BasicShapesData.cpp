#include "BasicShapesData.h"

// Triangle
const float TriangleVertices[] =
{
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.5f, 1.f,
};

const uint32_t TriangleIndices[] =
{
	0, 1, 2
};
// Triangle

// Square
const float SquareVertices[] = {
 0.5f,  0.5f, 0.0f,  1.f, 1.f, // top right
 0.5f, -0.5f, 0.0f,  1.f, 0.f, // bottom right
-0.5f, -0.5f, 0.0f,  0.f, 0.f, // bottom left
-0.5f,  0.5f, 0.0f,  0.f, 1.f  // top left 
};

const uint32_t SquareIndices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};
// Square

const float* BasicShapesData::GetTriangleVertices()
{
	return TriangleVertices;
}

const uint32_t* BasicShapesData::GetTriangleIndices()
{
	return TriangleIndices;
}

int32_t BasicShapesData::GetTriangleVerticesCount()
{
	return sizeof(TriangleVertices) / sizeof(float);
}

int32_t BasicShapesData::GetTriangleIndicesCount()
{
	return sizeof(TriangleIndices) / sizeof(uint32_t);
}

const float* BasicShapesData::GetSquareVertices()
{
	return SquareVertices;
}

const uint32_t* BasicShapesData::GetSquareIndices()
{
	return SquareIndices;
}

int32_t BasicShapesData::GetSquareVerticesCount()
{
	return sizeof(SquareVertices) / sizeof(float);
}

int32_t BasicShapesData::GetSquareIndicesCount()
{
	return sizeof(SquareIndices) / sizeof(uint32_t);
}
