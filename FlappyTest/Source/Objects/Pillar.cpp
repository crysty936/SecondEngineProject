#include "Pillar.h"
#include "Renderer/ShapesUtils/BasicShapes.h"

Pillar::Pillar()
{
	TopPillarDrawable = BasicShapes::CreateSquareObject();
	TopPillarDrawable->Model.Scale.y = 10.f;
	TopPillarDrawable->Model.Translation.y = 7.f;

	BottomPillarDrawable = BasicShapes::CreateSquareObject();
	BottomPillarDrawable->Model.Scale.y = 10.f;
	BottomPillarDrawable->Model.Translation.y = -7.f;

	Children.push_back(TopPillarDrawable);
	Children.push_back(BottomPillarDrawable);
}

Pillar::~Pillar() = default;

void Pillar::Init()
{
}

void Pillar::Tick(const float inDeltaT)
{
}

