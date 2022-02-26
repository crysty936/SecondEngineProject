#include "Model3D.h"

Model3D::Model3D() = default;
Model3D::~Model3D() = default;

void Model3D::Draw_Private() const
{
	for (const Mesh3D& childMesh : Meshes)
	{
		childMesh.Draw(Shader);
	}
}
