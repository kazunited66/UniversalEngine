#pragma once
//External Libs
#include <GLM/vec3.hpp>

struct USLight
{
	USLight() {
		colour = glm::vec3(1.0f);
		intensity = 1.0f;
	}

	virtual ~USLight() = default;
	glm::vec3 colour;
	float intensity;
};

struct USDirLight : public USLight {
	USDirLight() {
		ambient = glm::vec3(0.01f);
		direction = glm::vec3(0.0f);

	}

	glm::vec3 ambient;
	glm::vec3 direction;
};

struct USPointLight : public USLight {
	USPointLight() {
		position = glm::vec3(0.0f);
		linear = 0.045f;
		quadratic = 0.0075f;

	}
	glm::vec3 position;

	//fall off values fir how far the line can reach 
	float linear;
	float quadratic;

};