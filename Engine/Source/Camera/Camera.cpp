#include "Camera/Camera.h"
#include "Controller/Controller.h"
#include "glm/gtc/type_ptr.hpp"

Camera::Camera()
= default;

Camera::~Camera()
= default;

void Camera::Init()
{

}

void Camera::Tick(const float inDeltaT)
{
	
}

glm::mat4 Camera::GetLookAt()
{
	//glm::lookAt()

	constexpr glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//glm::vec3 euler = glm::eulerAngles(Model.NewRotation);
	glm::vec3 euler = Model.Rotation;

	// Inverse camera rotation is direction
	glm::vec3 cameraFront = glm::normalize(-euler);
 	glm::vec3 right = glm::normalize(glm::cross(globalUp, cameraFront));
 	glm::vec3 cameraUp = glm::cross(cameraFront, right);
 
	// Rotation Matrix
 	float firstMatrixA[16] =
 	{
 		right.x,		right.y,		right.z,		0,
 		cameraUp.x,		cameraUp.y,		cameraUp.z,		0,
 		cameraFront.x,	cameraFront.y,	cameraFront.z,		0,
 		0,				0,				0,				1
 	};
 
 	glm::mat4 rotationMatrix = glm::mat4(1.f);
 	memcpy(glm::value_ptr(rotationMatrix), firstMatrixA, sizeof(glm::mat4));
 	rotationMatrix = glm::transpose(rotationMatrix);

	// Translation Matrix
	glm::vec3 translation = Model.Translation;
	float SecondMatrixA[16] =
	{
		1,		0,		0,		-translation.x,
		0,		1,		0,		-translation.y,
		0,		0,		1,		-translation.z,
		0,		0,		0,		1
	};

	glm::mat4 translationMatrix = glm::mat4(1.f);
	memcpy(glm::value_ptr(translationMatrix), SecondMatrixA, sizeof(glm::mat4));
	translationMatrix = glm::transpose(translationMatrix);

	glm::mat4 lookAt =  translationMatrix * rotationMatrix;

	return lookAt;
}

