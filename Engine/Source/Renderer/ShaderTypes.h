#pragma once

struct SPointLight {
	glm::vec4 position;

	float linear;
	float quadratic;
	float padding1;
	float padding2;

	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
};

