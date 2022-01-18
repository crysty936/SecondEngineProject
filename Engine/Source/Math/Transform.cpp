#include "Transform.h"

glm::mat4 Transform::GetModel()
{
	glm::mat4 model(1.f);

	model = glm::translate(model, Translation);
	model = glm::rotate(model, Rotation.x, glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, Rotation.y, glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, Rotation.z, glm::vec3(0.f, 0.f, 1.f));

	model = glm::scale(model, Scale);

	return model;
}

