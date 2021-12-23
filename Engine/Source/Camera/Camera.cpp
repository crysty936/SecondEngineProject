#include "Camera/Camera.h"
#include "Controller/Controller.h"

Camera::Camera()
{
	// Preferrably camera should be constructed by receiving a controller
	// because multiple cameras can be tied to one controller but until that is necessary,
	// the camera creates it's own controller
	OwnedController = eastl::make_shared<class Controller>();
}

Camera::~Camera()
= default;

void Camera::Tick(const float inDeltaT)
{
	
}

