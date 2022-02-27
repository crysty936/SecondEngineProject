#pragma once
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "EASTL/vector.h"
#include "Renderer/OpenGL/Buffer/VertexArrayObject.h"
#include "Math/Transform.h"

enum class MeshType
{
	DrawElements,
	DrawArrays
};

class Mesh3D
{
public:
	Mesh3D() = default;
	~Mesh3D() = default;

	void Draw(const class OpenGLShader& inShader) const;
	void DeleteObject();

public:
	MeshType DrawType{ MeshType::DrawElements };
	
	// Don't need to keep them in memory as vertex and index buffer
	// are created when mesh is constructed
// 	eastl::vector<Vertex> Vertices;
// 	eastl::vector<uint32_t> Indices;
	eastl::vector<OpenGLTexture> Textures;
	VertexArrayObject ObjectVAO;
};