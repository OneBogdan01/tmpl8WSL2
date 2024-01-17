#include "LightManager.h"

#include "game.h"


void LightManager::SetLightProperties(Shader& shader)
{
	shader.Bind();
	shader.SetVec3("dirLight.direction", sunDir);
	shader.SetVec3("dirLight.specular", sunSpecular);
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		pointLights[i].SetLightProperties(shader, i);
	}
	shader.Unbind();
}

void LightManager::SetLightPosition(Shader& shader)
{
	shader.Bind();
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		pointLights[i].SetPosition(shader, i);
	}
	shader.Unbind();
}

LightManager::LightManager(Shader* lightShader) :
	lightShader(lightShader), pointLights{}, lightMesh()
{
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		PointLight& pointLight = pointLights[i];
		//some random colors for now
		pointLight.position = glm::vec3(i * 3.0f - 3.0f, 6.0f, 20.0f);
		pointLight.diffuse = (glm::vec3(i * .2, 0.0f, (4 - i) * .2));
	}
	lightMesh.Init();
}

void LightManager::SetPositionPointLight(size_t index, glm::vec3& pos)
{
	pointLights[index].position = pos;
}

void LightManager::Draw()
{
	lightShader->Bind();
	lightShader->SetMat4x4("projection", Game::perspective);
	lightShader->SetMat4x4("view", Game::view);

	for (auto& pointLight : pointLights)
	{
		glm::mat4 sunModel = glm::mat4(1.0f);

		sunModel = glm::translate(sunModel, pointLight.position);
		sunModel = glm::scale(sunModel, glm::vec3(0.4f));

		lightShader->SetMat4x4("model", sunModel);
		lightShader->SetVec3("color", pointLight.diffuse);

		lightMesh.Draw();
	}
	lightShader->Unbind();
}

glm::vec3 LightManager::GetSunDir() const
{
	return sunDir;
}
