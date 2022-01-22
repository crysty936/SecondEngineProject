#include "CeilingAndFloor.h"
#include "Math/Transform.h"
#include "Renderer/ShapesUtils/BasicShapes.h"

const float ScaleMultiplier = 35.f;
const float HeightOffset = 5.f;

CeilingAndFloor::CeilingAndFloor()
	: Ceiling{ nullptr }, Floor{ nullptr }
{
	Transform ceilingModel;
	Transform floorModel;

	ceilingModel.Translation.y = HeightOffset;
	floorModel.Translation.y = -HeightOffset;
	ceilingModel.Scale.x = ScaleMultiplier;
	floorModel.Scale.x = ScaleMultiplier;

	Ceiling = BasicShapes::CreateSquareObject();
	Floor = BasicShapes::CreateSquareObject();

	Ceiling->Model = ceilingModel;
	Floor->Model = floorModel;

	AddChild(Ceiling);
	AddChild(Floor);
}

CeilingAndFloor::~CeilingAndFloor() = default;

void CeilingAndFloor::Init()
{
	
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

