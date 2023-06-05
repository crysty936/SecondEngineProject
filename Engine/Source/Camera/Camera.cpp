#include "Camera/Camera.h"
#include "Controller/ControllerBase.h"
#include "glm/gtc/type_ptr.hpp"
#include "Logger/Logger.h"
#include "Entity/TransformObject.h"

Camera::Camera()
	: Entity("Camera") 
{}

Camera::~Camera() = default;

void Camera::Init()
{

}

void Camera::Tick(const float inDeltaT)
{
	
}

void Camera::Move(MovementDirection inDirection, const float inSpeed) 
{
	constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	const glm::quat absRotation = GetAbsoluteTransform().Rotation;

	const glm::quat qF = absRotation * glm::quat(0, 0, 0, -1) * glm::conjugate(absRotation);
	glm::vec3 Front = { qF.x, qF.y, qF.z };
	glm::vec3 Right = glm::normalize(glm::cross(Front, up));

	Front *= MouseMoveSensitivity;
	Right *= MouseMoveSensitivity;

	if (TransformObjPtr parentShared = Parent.lock())
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


void Camera::SetMovementDelegates(ControllerBase& inController)
{
	inController.OnMouseMoved().BindRaw(this, &Camera::OnMousePosChanged);
	inController.OnMouseScroll().BindRaw(this, &Camera::OnMouseScrollChanged);
}

void Camera::OnMouseScrollChanged(const float inNewY)
{
	MouseMoveSensitivity = glm::max(0.01f, MouseMoveSensitivity + inNewY / 10.f);
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

	const float yawOffset = (inNewYaw - MouseLastYaw) * MouseLookSensitivity;
	const float pitchOffset = ((-1.f) * (inNewPitch - MouseLastPitch)) * MouseLookSensitivity;


	MouseLastYaw = inNewYaw;
	MouseLastPitch = inNewPitch;

// 	LOG_INFO("Mouse yaw offset %f", yawOffset);
// 	LOG_INFO("Mouse yaw %f", inNewYaw);

// 	const float sensitivity = 0.1f;
// 
// 	Yaw += yawOffset * sensitivity;
// 	Pitch += pitchOffset * sensitivity;

//   	// Yaw
//   	glm::quat aroundY = glm::angleAxis(glm::radians(-Yaw), glm::vec3(0, 1, 0));
//   
//   	// Pitch
//   	glm::quat aroundX = glm::angleAxis(glm::radians(Pitch), glm::vec3(1, 0, 0));
// 
// 	glm::quat result = aroundY * aroundX;
// 
// 	SetRotation(glm::eulerAngles(result));


// 	glm::quat pitchAdditiveRotation = glm::angleAxis(glm::radians(pitchOffset), glm::vec3(1.f, 0.f, 0.f));
// 	glm::quat yawAdditiveRotation = glm::angleAxis(glm::radians(-yawOffset), glm::vec3(0.f, 1.f, 0.f));

	//Model.Rotation = pitchAdditiveRotation * Model.Rotation ;
	//Model.Rotation = Model.Rotation * pitchAdditiveRotation;
 

 	if (TransformObjPtr parentShared = Parent.lock())
 	{
 		parentShared->Rotate(-yawOffset, glm::vec3(0.f, 1.f, 0.f));
 	}

	//Rotate(-yawOffset, glm::vec3(0.f, 1.f, 0.f));
	Rotate(pitchOffset, glm::vec3(1.f, 0.f, 0.f));
}

glm::mat4 Camera::GetLookAt()
{
// 	//https://www.3dgep.com/understanding-the-view-matrix/
// 	
//  	constexpr glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
//  
// 	const Transform& absTransf = GetAbsoluteTransform();
// 	//glm::vec3 euler = glm::eulerAngles(Model.Rotation);
//  	//glm::vec3 euler = Model.Rotation;
//  	glm::vec3 euler = glm::eulerAngles(absTransf.Rotation);
//  
//  	// Inverse camera rotation is direction
//  	// ! This is not a camera front, it's a camera direction that includes all axes 
// 	//glm::vec3 cameraDir = glm::normalize(-euler);
//  	glm::vec3 cameraDir = glm::normalize(absTransf.Rotation * glm::vec3(0.f, 0.f, 1.f));
//   	glm::vec3 right = glm::normalize(glm::cross(globalUp, cameraDir));
//   	//glm::vec3 right = glm::normalize(glm::cross(cameraDir, globalUp));
//   	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDir, right));
//   	//glm::vec3 cameraUp = glm::normalize(glm::cross(right, cameraDir));
//  
//  
//  	// Rotation Matrix
//  	/**
//  	* Normally this matrix would be like this:
//  	*  	float firstMatrixA[16] =
//   		{
//   			right.x,		right.y,		right.z,		0,
//   			cameraUp.x,		cameraUp.y,		cameraUp.z,		0,
//   			cameraDir.x,	cameraDir.y,	cameraDir.z,		0,
//   			0,				0,				0,				1
//   		};
//  		
//  		And we would need to do:
//  		//rotationMatrix = glm::transpose(rotationMatrix);
//  		Because it's transpose is equal to it's inverse because it the matrix
//  		is orthonormalized
//  
//  		However, we can build it directly transposed
//  	 */
//   	float firstMatrixA[16] =
//   	{
// 		right.x,		cameraUp.x,		cameraDir.x,		0,
// 		right.y,		cameraUp.y,		cameraDir.y,		0,
// 		right.z,		cameraUp.z,		cameraDir.z,		0,
//   		0,				0,				0,				1
//   	};
//   
//   	glm::mat4 rotationMatrix = glm::mat4(1.f);
//   	memcpy(glm::value_ptr(rotationMatrix), firstMatrixA, sizeof(glm::mat4));
//  
//  	// Translation Matrix
//  	// Position is negated so that camera is at origin
//  	// 
//  	// Translation inverse is equal to T(-v) T(v)^-1 == T(-v)
// 
// 	//Also, inverse of a TranslationRotation matrix (X)^-1 = (T(t) * R)^-1 = R^t * T(-t), so R^-1 * T^-1, open parantheses mean inverse of operations and inverse of each
//  	const glm::vec3 translation = absTransf.Translation;
//   	float SecondMatrixA[16] =
//   	{
//   		1,		0,		0,		-translation.x,
//   		0,		1,		0,		-translation.y,
//   		0,		0,		1,		-translation.z,
//   		0,		0,		0,		1
//   	};
//  
// //  	 	float SecondMatrixA[16] =
// //   	{
// //   		1,		0,		0,		0,
// //   		0,		1,		0,		0,
// //   		0,		0,		1,		0,
// //   		-translation.x,		-translation.y,		-translation.z,		1
// //   	};
// 
//  	glm::mat4 translationMatrix = glm::mat4(1.f);
//  	memcpy(glm::value_ptr(translationMatrix), SecondMatrixA, sizeof(glm::mat4));
//  	translationMatrix = glm::transpose(translationMatrix);
// 
//  
// 	 //if this is made like, where the translation is supposed to be, then there's no need for transposing
// 
//  	// Combine to get TRS(without the scale)
//  	// The order of multiplication is reversed because the matrices are
//  	// inverted.
//  	glm::mat4 lookAt = rotationMatrix /** translationMatrix*/;
// 
// 	return lookAt;

	const glm::mat4 viewMatrix = glm::inverse(GetAbsoluteTransform().GetMatrix());

 	return viewMatrix;
}

