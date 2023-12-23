#pragma once
#include "glm/vec3.hpp"


class Shader;

class PointLight
{
public:
	void SetLightProperties(Shader& lightingShader, size_t index);


	void SetConstant(float newConstant);

	void SetLinear(float newLinear);

	void SetQuadratic(float newQuadratic);
	glm::vec3 position;
	glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

private:
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};
