#pragma once
#include "Renderer/Drawable/DrawableBase.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/Material/RenderMaterial.h"
#include "Mesh3D.h"
#include "Entity/TransformObject.h"
#include "Renderer/Drawable/DrawableBase.h"

// Each mesh node has a number of meshes. Those meshes don't have their own location as it's the
// Node that holds the transform for all of them. One Mesh Node to many Render Commands, each Render Command is a mesh

struct MeshNode : public TransformObject
{
	eastl::vector<Mesh3D> Meshes;
};

class Model3D : public TransformObject, public IDrawable
{
public:
	Model3D();
	virtual ~Model3D();

	//virtual void SetupDrawCommand() override;

protected:
	//virtual void Draw_Private() const override;
	//void DrawMeshesRecursive(const MeshNode& inNode, const Transform inParentWorldTransform) const;

protected:
	//MeshNode RootMeshNode;
};