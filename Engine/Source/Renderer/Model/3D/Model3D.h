#pragma once
#include "Renderer/Drawable/DrawableBase.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/Drawable/Material.h"
#include "Mesh3D.h"

struct MeshNode
{
	Transform RelativeTranfs{};
	eastl::vector<Mesh3D> Meshes;
	eastl::vector<MeshNode> Children;
};

class Model3D : public DrawableBase
{
public:
	Model3D();
	virtual ~Model3D();

	inline void AddMesh(Mesh3D&& inMesh) { Meshes.push_back(inMesh); }

protected:
	virtual void Draw_Private() const override;
	void DrawMeshesRecursive(const MeshNode& inNode, const Transform inParentWorldTransform) const;

protected:
	eastl::vector<Mesh3D> Meshes;
	MeshNode RootMeshNode;
};