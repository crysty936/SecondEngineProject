#pragma once
#include "VertexBuffer.h"

class VAO
{
public:
	VAO(VertexBuffer& inBuffer);
	VAO();
	~VAO();

	void Bind() const;
	void Unbind() const;
	void DeleteBuffers();
	inline const VertexBuffer& GetVertexBuffer() const { return VBuffer; }

private:
	uint32_t Handle;
	VertexBuffer VBuffer;
};