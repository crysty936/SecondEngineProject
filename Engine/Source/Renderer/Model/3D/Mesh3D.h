#pragma once
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "EASTL/vector.h"
#include "Renderer/OpenGL/Buffer/VertexArrayObject.h"
#include "Math/Transform.h"
#include "EASTL/shared_ptr.h"

class Mesh3D
{
public:
	Mesh3D();
	~Mesh3D();
};