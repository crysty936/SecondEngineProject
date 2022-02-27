#include "Model3D.h"

Model3D::Model3D() = default;
Model3D::~Model3D() = default;

void Model3D::Draw_Private() const
{
	DrawMeshesRecursive(RootMeshNode, GetAbsoluteTransform());

// 	for (const Mesh3D& childMesh : Meshes)
// 	{
// 		childMesh.Draw(Shader, GetAbsoluteTransform());
// 	}
}


void Model3D::DrawMeshesRecursive(const MeshNode& inNode, const Transform inParentWorldTransform) const
{
	for (const MeshNode& childNode : inNode.Children)
	{
		// TODO: No reason to calculate this each time, especially if the mesh is static
		const Transform absoluteTransform = childNode.RelativeTranfs * inParentWorldTransform;
		DrawMeshesRecursive(childNode, absoluteTransform);
	}

	Shader.SetUniformValue4fv("model", inParentWorldTransform.GetMatrix());

	for (const Mesh3D& mesh : inNode.Meshes)
	{
		mesh.Draw(Shader);
	}

}