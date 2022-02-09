#include "Camera/Camera.h"
#include "Controller/Controller.h"
#include "glm/gtc/type_ptr.hpp"
#include "Logger/Logger.h"

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

void Camera::Move(MovementDirection inDirection, const float inSpeed) 
{
	constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::quat qF = Model.Rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(Model.Rotation);
	glm::vec3 Front = { qF.x, qF.y, qF.z };
	glm::vec3 Right = glm::normalize(glm::cross(Front, up));


	switch (inDirection)
	{
	case MovementDirection::Forward:
		Model.Translation += Front * inSpeed;
		break;
	case MovementDirection::Back:
		Model.Translation -= Front * inSpeed;
		break;
	case MovementDirection::Right:
		Model.Translation += Right * inSpeed;
		break;
	case MovementDirection::Left:
		Model.Translation -= Right * inSpeed;
		break;
	default:
		break;
	}
}


void Camera::SetMovementDelegates(Controller& inController)
{
	MouseMovedDelegate del = MouseMovedDelegate::CreateRaw(this, &Camera::OnMousePosChanged);

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
	const float pitchOffset = (-1.f) * (inNewPitch - MouseLastPitch);

	MouseLastYaw = inNewYaw;
	MouseLastPitch = inNewPitch;

	const float sensitivity = 0.1f;

	Yaw += yawOffset * sensitivity;
	Pitch += pitchOffset * sensitivity;

 	// Yaw
 	glm::quat aroundY = glm::angleAxis(glm::radians(-Yaw), glm::vec3(0, 1, 0));
 
 	// Pitch
 	glm::quat aroundX = glm::angleAxis(glm::radians(Pitch), glm::vec3(1, 0, 0));

	glm::quat rotation = aroundY * aroundX;	
	glm::vec3 euler = glm::eulerAngles(rotation);
	Logger::Get().Print("X: %f  Y: %f  Z:  %f", Severity::Info, euler.x, euler.y, euler.z);

	Model.Rotation = rotation;
}

glm::mat4 Camera::GetLookAt()
{
	//https://www.3dgep.com/understanding-the-view-matrix/
	
// 	constexpr glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
// 
// 	//glm::vec3 euler = glm::eulerAngles(Model.Rotation);
// 	glm::vec3 euler = Model.Rotation;
// 
// 	// Inverse camera rotation is direction
// 	glm::vec3 cameraFront = glm::normalize(-euler);
//  	glm::vec3 right = glm::normalize(glm::cross(globalUp, cameraFront));
//  	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraFront, right));
// 
// 
// 	// Rotation Matrix
// 	/**
// 	* Normally this matrix would be like this:
// 	*  	float firstMatrixA[16] =
//  		{
//  			right.x,		right.y,		right.z,		0,
//  			cameraUp.x,		cameraUp.y,		cameraUp.z,		0,
//  			cameraFront.x,	cameraFront.y,	cameraFront.z,		0,
//  			0,				0,				0,				1
//  		};
// 		
// 		And we would need to do:
// 		//rotationMatrix = glm::transpose(rotationMatrix);
// 		Because it's transpose is equal to it's inverse because it the matrix
// 		is orthonormalized
// 
// 		However, we can build it directly transposed
// 	 */
//  	float firstMatrixA[16] =
//  	{
//  		right.x,		cameraUp.x,		cameraFront.x,		0,
// 		right.y,		cameraUp.y,		cameraFront.y,		0,
// 		right.z,		cameraUp.z,		cameraFront.z,		0,
//  		0,				0,				0,				1
//  	};
//  
//  	glm::mat4 rotationMatrix = glm::mat4(1.f);
//  	memcpy(glm::value_ptr(rotationMatrix), firstMatrixA, sizeof(glm::mat4));
// 
// 	// Translation Matrix
// 	// Position is negated which makes it equivalent to
// 	// the inverse of the translation matrix.
// 	// T(v)^-1 == T(-v)
// 	glm::vec3 translation = Model.Translation;
// 	float SecondMatrixA[16] =
// 	{
// 		1,		0,		0,		-translation.x,
// 		0,		1,		0,		-translation.y,
// 		0,		0,		1,		-translation.z,
// 		0,		0,		0,		1
// 	};
// 
// 	glm::mat4 translationMatrix = glm::mat4(1.f);
// 	memcpy(glm::value_ptr(translationMatrix), SecondMatrixA, sizeof(glm::mat4));
// 	translationMatrix = glm::transpose(translationMatrix);
// 
// 	// Combine to get TRS(without the scale)
// 	// The order of multiplication is reversed because the matrices are
// 	// inverted.
// 	glm::mat4 lookAt = rotationMatrix * translationMatrix;

	//return lookAt;

	return glm::inverse(Model.GetModel());
}

