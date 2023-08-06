#pragma once
#include "Renderer/RHI/Resources/RHIShader.h"
#include <stdint.h>

class GLShader : public RHIShader
{
public:
	GLShader(const uint32_t inGLHandle, const uint32_t inVertexHandle, const uint32_t inFragmentHandle = 0, const uint32_t inGeometryHandle = 0);

public:
	uint32_t MainHandle = 0;

	uint32_t VertexHandle = 0;
	uint32_t FragmentHandle = 0;
	uint32_t GeometryHandle = 0;
};