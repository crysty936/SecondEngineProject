#pragma once
#include "VertexBuffer.h"

class VAO
{
public:
	VAO(VertexBuffer& inBuffer);
	~VAO();

	void Bind();
	void Unbind();
	void DeleteBuffers();
	inline const VertexBuffer& GetVertexBuffer() const { return VBuffer; }

private:
	uint32_t Handle;
	VertexBuffer VBuffer;
};