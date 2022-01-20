#include "Transform.h"

glm::mat4 Transform::GetModel()
{
	glm::mat4 model(1.f);

	// TODO: Find a better operation, maybe from the Camera Class, rather than doing the same one 3 times
	model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.f, 0.f, 1.f));

	model = glm::translate(model, Translation);

	model = glm::scale(model, Scale);

	return model;
}

