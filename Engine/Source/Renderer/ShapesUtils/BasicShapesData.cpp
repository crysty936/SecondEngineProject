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
// CCW Vertices with secondary diagonal split drawing
const float SquareVertices[] = {
 0.5f,  0.5f, 0.0f,  1.f, 1.f, // top right
-0.5f,  0.5f, 0.0f,  0.f, 1.f,  // top left 
 0.5f, -0.5f, 0.0f,  1.f, 0.f, // bottom right
-0.5f, -0.5f, 0.0f,  0.f, 0.f, // bottom left
};

const uint32_t SquareIndices[] = {
	0, 1, 2,   // first triangle
	1, 3, 2    // second triangle
};

// CCW Vertices with main diagonal split drawing
// const float SquareVertices[] = {
//  0.5f,  0.5f, 0.0f,  1.f, 1.f, // top right
// -0.5f, -0.5f, 0.0f,  0.f, 0.f, // bottom left
//  0.5f, -0.5f, 0.0f,  1.f, 0.f, // bottom right
// -0.5f,  0.5f, 0.0f,  0.f, 1.f  // top left 
// };
// 
// const uint32_t SquareIndices[] = {
// 	0, 1, 2,   // first triangle
// 	1, 0, 3    // second triangle
// };


// CW Vertices with CCW Indexes
// const float SquareVertices[] = {
//  0.5f,  0.5f, 0.0f,  1.f, 1.f, // top right
//  0.5f, -0.5f, 0.0f,  1.f, 0.f, // bottom right
// -0.5f, -0.5f, 0.0f,  0.f, 0.f, // bottom left
// -0.5f,  0.5f, 0.0f,  0.f, 1.f  // top left 
// };

// const uint32_t SquareIndices[] = {
// 	3, 1, 0,   // first triangle
// 	3, 2, 1    // second triangle
//};
// Square

// Cube

// CCW Cube vertices
// The vertices are defined as if you would be looking at the cube straight on, for example, the front and back are reversed,
// the side vertices are defined all on the left plane, with the x stuck to -0.5, is if that plane is perpendicular to the camera and so on
const float CubeVertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left  
};

// TODO: Add the indices of the cube to get rid of Draw Arrays
const uint32_t CubeIndices[] = {
	0, 1, 2,
	1, 2, 3



};

// Cube

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

const float* BasicShapesData::GetCubeVertices()
{
	return CubeVertices;
}

const uint32_t* BasicShapesData::GetCubeIndices()
{
	return CubeIndices;
}

int32_t BasicShapesData::GetCubeVerticesCount()
{
	return sizeof(CubeVertices) / sizeof(float);
}

int32_t BasicShapesData::GetCubeIndicesCount()
{
	return 36;

	// TODO: Add the indices of the cube to get rid of Draw Arrays
	return sizeof(CubeIndices) / sizeof(uint32_t);
}
