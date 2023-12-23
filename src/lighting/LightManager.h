#pragma once
#include "PointLight.h"
#include "Sun.h"
#include "glm/ext/quaternion_geometric.hpp"
constexpr int MAX_POINT_LIGHTS = 4;

class LightManager
{
public:
	void SetLightProperties(Shader& shader);

	LightManager(Shader* lightShader);
	void Draw();

	glm::vec3 GetSunDir() const;

private:
	glm::vec3 sunDir = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
	glm::vec3 sunSpecular = glm::vec3(1.0);

	Shader* lightShader = nullptr;
	PointLight pointLights[MAX_POINT_LIGHTS];
	Sun lightMesh;
};
