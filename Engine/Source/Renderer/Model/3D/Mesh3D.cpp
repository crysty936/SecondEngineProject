#include "Mesh3D.h"
#include "Renderer/OpenGL/OpenGLShader.h"

void Mesh3D::DeleteObject()
{
// 	ObjectVAO.DeleteBuffers();
// 	Shader.DeleteShader();
// 	for (OpenGLTexture& tex : Textures)
// 	{
// 		tex.DeleteTexture();
// 	}
}

void Mesh3D::Draw(const OpenGLShader& inShader) const
{
	uint32_t indicesCount = ObjectVAO.VBuffer.GetIndicesCount();

	for (const OpenGLTexture& tex : Textures)
	{
		tex.Bind();
	}

	ObjectVAO.Bind();
	switch (DrawType)
	{
	case MeshType::DrawElements:
	{
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		break;
	}
	case MeshType::DrawArrays:
	{
		glDrawArrays(GL_TRIANGLES, 0, indicesCount);
		break;
	}
	}

	ObjectVAO.Unbind();

	for (const OpenGLTexture& tex : Textures)
	{
		tex.Unbind();
	}
}
