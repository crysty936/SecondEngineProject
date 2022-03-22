#pragma once
#include "VertexBuffer.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void SetupState() const;
	void Bind() const;
	void Unbind() const;
	void DeleteBuffers();

	VertexBuffer VBuffer{};

	mutable uint32_t Handle{ 0 };
	mutable bool bReadyForDraw = false;
};