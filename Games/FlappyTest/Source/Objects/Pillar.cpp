#include "Pillar.h"
#include "Renderer/ShapesUtils/BasicShapes.h"

Pillar::Pillar()
	: TopPillarDrawable(), BottomPillarDrawable()
{

}

Pillar::~Pillar()
{

}

void Pillar::Init()
{
	Entity::Init();

	TopPillarDrawable = BasicShapes::CreateSquareObject();
	TopPillarDrawable->Model.Scale.y = 10.f;
	TopPillarDrawable->Model.Translation.y = 7.f;

	BottomPillarDrawable = BasicShapes::CreateSquareObject();
	BottomPillarDrawable->Model.Scale.y = 10.f;
	BottomPillarDrawable->Model.Translation.y = -7.f;

	TopPillarDrawable->SetParent(shared_from_this());
	BottomPillarDrawable->SetParent(shared_from_this());
}

void Pillar::Tick(const float inDeltaT)
{
}

