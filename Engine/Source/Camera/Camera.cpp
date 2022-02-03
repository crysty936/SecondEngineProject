#include "Camera/Camera.h"
#include "Controller/Controller.h"
#include "glm/gtc/type_ptr.hpp"

Camera::Camera()
	: FirstMouse{true}
	, MouseLastYaw{0.0f}
	, MouseLastPitch{0.0f}
{}

Camera::~Camera()
= default;

void Camera::Init()
{

}

void Camera::Tick(const float inDeltaT)
{
	
}

void Camera::SetMovementDelegates(Controller& inController)
{
	MouseMovedDelegate del = MouseMovedDelegate::CreateRaw(this, &Camera::OnMousePosChanged);
	del.Execute(0.f, 0.f);
	inController.AddMouseListener(del);
}

void Camera::OnMousePosChanged(const float inNewYaw, const float inNewPitch)
{
	if (FirstMouse)
	{
		FirstMouse = false;

		MouseLastYaw = inNewYaw;
		MouseLastPitch = inNewPitch;
	}

	const float yawOffset = inNewYaw - MouseLastYaw;
	const float pitchOffset = inNewPitch - MouseLastPitch;

	MouseLastYaw = inNewYaw;
	MouseLastPitch = inNewPitch;

	const float sensitivity = 0.1f;
	
	Yaw += (yawOffset * sensitivity);
	Pitch += (pitchOffset * sensitivity);

// 	const float newYaw = Model.Rotation.x + yawOffset;;
// 	const float newPitch = Model.Rotation.y + pitchOffset;

	glm::vec3 newRotation;
	newRotation.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	newRotation.y = sin(glm::radians(Pitch));
	newRotation.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Model.Rotation = glm::normalize(newRotation);
}

glm::mat4 Camera::GetLookAt()
{
	constexpr glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//glm::vec3 euler = glm::eulerAngles(Model.NewRotation);
	glm::vec3 euler = Model.Rotation;

	// Inverse camera rotation is direction
	glm::vec3 cameraFront = glm::normalize(-euler);
 	glm::vec3 right = glm::normalize(glm::cross(globalUp, cameraFront));
 	glm::vec3 cameraUp = glm::cross(cameraFront, right);
 
	//return glm::lookAt(Model.Translation, glm::normalize(-Model.Rotation), cameraUp);

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

