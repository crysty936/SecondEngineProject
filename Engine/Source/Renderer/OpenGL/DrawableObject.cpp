#include "DrawableObject.h"

DrawableObject::DrawableObject(VAO& inVAO, OpenGLShader& inShader)
	: ObjectVAO{ inVAO }, Shader{ inShader }
{
}

DrawableObject::DrawableObject()
{}

DrawableObject::~DrawableObject()
{}

void DrawableObject::Draw() const
{
	uint32_t indicesCount = ObjectVAO.GetVertexBuffer().GetIndicesCount();

	const eastl::vector<OpenGLTexture>& textures = GetTextures();
	for (const OpenGLTexture& tex : textures)
	{
		tex.Bind();
	}

	ObjectVAO.Bind();
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

	Shader.UnBind();
	ObjectVAO.Unbind();
	for (const OpenGLTexture& tex : textures)
	{
		tex.Unbind();
	}
}

void DrawableObject::AddTexture(OpenGLTexture inTexture)
{
	Textures.push_back(inTexture);
}

void DrawableObject::DeleteObject()
{
	ObjectVAO.DeleteBuffers();
	Shader.DeleteShader();
	for (OpenGLTexture& tex : Textures)
	{
		tex.DeleteTexture();
	}
}

void DrawableObject::Init()
{
	
}
