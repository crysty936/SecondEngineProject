#include "CeilingAndFloor.h"
#include "Math/Transform.h"
#include "Renderer/ShapesUtils/BasicShapes.h"

CeilingAndFloor::CeilingAndFloor()
	: Ceiling{ nullptr }, Floor{ nullptr }
{
	Transform ceilingModel;
	Transform floorModel;

	ceilingModel.Translation.y = 5.f;
	floorModel.Translation.y = -5.f;
	ceilingModel.Scale.x = 20.f;
	floorModel.Scale.x = 20.f;

	Ceiling = BasicShapes::CreateSquareObject();
	Floor = BasicShapes::CreateSquareObject();

	Ceiling->Model = ceilingModel;
	Floor->Model = floorModel;


	Children.push_back(Ceiling);
	Children.push_back(Floor);
}

CeilingAndFloor::~CeilingAndFloor() = default;

void CeilingAndFloor::Init()
{
	
}

void CeilingAndFloor::Tick(const float inDeltaT)
{
	
}

