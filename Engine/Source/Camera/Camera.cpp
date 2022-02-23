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

	glm::quat absRotation = GetAbsoluteTransform().Rotation;

	glm::quat qF = absRotation * glm::quat(0, 0, 0, -1) * glm::conjugate(absRotation);
	glm::vec3 Front = { qF.x, qF.y, qF.z };
	glm::vec3 Right = glm::normalize(glm::cross(Front, up));

	if (EntityPtr parentShared = Parent.lock())
	{
		glm::vec3 movementVector(0.f);

		switch (inDirection)
		{
		case MovementDirection::Forward:
			movementVector = Front * inSpeed;
			break;
		case MovementDirection::Back:
			movementVector -= Front * inSpeed;
			break;
		case MovementDirection::Right:
			movementVector = Right * inSpeed;
			break;
		case MovementDirection::Left:
			movementVector -= Right * inSpeed;
			break;
		default:
			break;
		}

		parentShared->Move(movementVector);
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

		return;
	}

	const float yawOffset = inNewYaw - MouseLastYaw;
	const float pitchOffset = (-1.f) * (inNewPitch - MouseLastPitch);

	MouseLastYaw = inNewYaw;
	MouseLastPitch = inNewPitch;

// 	const float sensitivity = 0.1f;
// 
// 	Yaw += yawOffset * sensitivity;
// 	Pitch += pitchOffset * sensitivity;

//  	// Yaw
//  	glm::quat aroundY = glm::angleAxis(glm::radians(-Yaw), glm::vec3(0, 1, 0));
//  
//  	// Pitch
//  	glm::quat aroundX = glm::angleAxis(glm::radians(Pitch), glm::vec3(1, 0, 0));

// 
// 	glm::quat pitchAdditiveRotation = glm::angleAxis(glm::radians(pitchOffset), glm::vec3(1.f, 0.f, 0.f));
// 	glm::quat yawAdditiveRotation = glm::angleAxis(glm::radians(-yawOffset), glm::vec3(0.f, 1.f, 0.f));

	//Model.Rotation = pitchAdditiveRotation * Model.Rotation ;
	//Model.Rotation = Model.Rotation * pitchAdditiveRotation;
 

 	if (EntityPtr parentShared = Parent.lock())
 	{
 		parentShared->Rotate(-yawOffset, glm::vec3(0.f, 1.f, 0.f));
 	}

	//Rotate(-yawOffset, glm::vec3(0.f, 1.f, 0.f));
	Rotate(pitchOffset, glm::vec3(1.f, 0.f, 0.f));
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

	const glm::mat4 inverse = glm::inverse(GetAbsoluteTransform().GetMatrix());

	return inverse;
}

