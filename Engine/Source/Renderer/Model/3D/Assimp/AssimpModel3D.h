#pragma once
#include "Renderer/Model/3D/Model3D.h"
#include "EASTL/string.h"
#include "Core/EngineUtils.h"
#include "assimp/material.h"

class AssimpModel3D : public Model3D
{
public:
	AssimpModel3D(const eastl::string& inPath);
	virtual ~AssimpModel3D();

	virtual void CreateProxy() override;

	static void LoadModelToRoot(const eastl::string inPath, TransformObjPtr inParent);

private:
	eastl::string ModelPath;
};