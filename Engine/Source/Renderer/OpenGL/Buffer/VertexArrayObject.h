#pragma once
#include "VertexBuffer.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void SetupState();
	void Bind() const;
	void Unbind() const;
	void DeleteBuffers();

	uint32_t Handle{ 0 };
	VertexBuffer VBuffer{};
};