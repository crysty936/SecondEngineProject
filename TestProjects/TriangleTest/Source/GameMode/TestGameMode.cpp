#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"
#include "glm/common.hpp"
#include <stdlib.h>
#include "Renderer/Model/3D/Assimp/AssimpModel3D.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Core/ObjectCreation.h"
#include "Renderer/Drawable/MirrorQuad.h"

TestGameMode GameMode = {};

TestGameMode::TestGameMode()
	: GameModeBase()
{}

TestGameMode::~TestGameMode() = default;

void TestGameMode::Init()
{
 	triangle = ObjectCreation::NewObject<TriangleShape>();
 	triangle->Move(glm::vec3(0.f, 0.f, -10.f));
}

 void TestGameMode::Tick(float inDeltaT)
 {
	 triangle->Rotate(inDeltaT, glm::vec3(0.f, 0.f, 1.f));
 }
