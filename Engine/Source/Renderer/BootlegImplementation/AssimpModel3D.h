#pragma once
#include "Renderer/Drawable/DrawableBase.h"

class AssimpModel3D : public DrawableBase
{
public:
	AssimpModel3D(const eastl::string& inPath);
	virtual ~AssimpModel3D();


private:
	void LoadData(const eastl::string& inPath);


	void ProcessNode(const struct aiNode& inNode, const struct aiScene& inScene);
};