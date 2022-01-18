#include "Camera/Camera.h"
#include "Controller/Controller.h"

Camera::Camera()
{
	OwnedController = eastl::make_shared<class Controller>();
}

Camera::~Camera()
= default;

void Camera::Init()
{

}

void Camera::Tick(const float inDeltaT)
{
	
}

