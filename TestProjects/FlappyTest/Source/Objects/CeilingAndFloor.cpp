#include "CeilingAndFloor.h"
#include "Math/Transform.h"
#include "Renderer/ShapesUtils/BasicShapes.h"
#include "Pillar.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Math/MathUtils.h"

const float HeightOffset = 5.f;

const float CeilingAndFloor::EntityWidth = 80.f;

CeilingAndFloor::CeilingAndFloor()
	: Ceiling{ nullptr }, Floor{ nullptr }
{
}

CeilingAndFloor::~CeilingAndFloor() = default;

void CeilingAndFloor::Init()
{
	Entity::Init();

	Transform ceilingModel;
	Transform floorModel;

	ceilingModel.Translation.y = HeightOffset;
	floorModel.Translation.y = -HeightOffset;
	ceilingModel.Scale.x = EntityWidth;
	floorModel.Scale.x = EntityWidth;

	Ceiling = BasicShapes::CreateSquareObject();
	Floor = BasicShapes::CreateSquareObject();

	Ceiling->Model = ceilingModel;
	Floor->Model = floorModel;

	AddChild(Ceiling);
	AddChild(Floor);

	float pillarsOffset = -40.f;

	for (eastl::shared_ptr<Pillar>& pillar : Pillars)
	{
		const float offsetRandom = static_cast<float>(MathUtils::GetRandom(0, 10));
		pillarsOffset += 5.f + offsetRandom;

		pillar = CreatePillar();
		AddChild(pillar);

		pillar->Model.Translation.x -= pillarsOffset;
	}
}

void CeilingAndFloor::Tick(const float inDeltaT)
{

}

float CeilingAndFloor::GetFloorY()
{
	return (Transform{}).Translation.y - HeightOffset;
}

float CeilingAndFloor::GetCeilingY()
{
	return (Transform{}).Translation.y + HeightOffset;
}

void CeilingAndFloor::MovePillars()
{


}

eastl::shared_ptr<Pillar> CeilingAndFloor::CreatePillar()
{
	eastl::shared_ptr<Pillar> newPillar = eastl::make_shared<Pillar>();

	return newPillar;
}

