#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"
#include "glm/common.hpp"
#include <stdlib.h>
#include "Renderer/Model/3D/Assimp/AssimpModel3D.h"
#include "Renderer/ForwardRenderer.h"
#include "Core/ObjectCreation.h"
#include "Renderer/Drawable/MirrorQuad.h"

TestGameMode GameMode = {};

TestGameMode::TestGameMode()
	: GameModeBase()
{}

TestGameMode::~TestGameMode() = default;

void TestGameMode::Init()
{
 	triangle = ObjectCreation::NewObject<CubeShape>();
 	triangle->Move(glm::vec3(0.f, 0.f, -5.f));

 	//triangle2 = ObjectCreation::NewObject<TriangleShape>();
 	//triangle2->Move(glm::vec3(0.f, 0.f, -100.f));

}

 void TestGameMode::Tick(float inDeltaT)
 {
	 triangle->Rotate(inDeltaT * 50.f, glm::vec3(0.f, 1.f, 1.f));
	 //triangle2->Rotate(inDeltaT, glm::vec3(0.f, 0.f, -1.f));
 }
