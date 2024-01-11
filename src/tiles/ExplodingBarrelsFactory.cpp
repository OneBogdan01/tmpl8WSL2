#include "ExplodingBarrelsFactory.h"

#include "game.h"

ExplodingBarrelsFactory::ExplodingBarrelsFactory()
{
	modelShader = new Shader(
		"assets/shaders/ModelLoadingNoInstance.vert",
		"assets/shaders/ModelLoading.frag");
	SetMaterialProperties();
	Game::lightManager->SetLightProperties(*modelShader);
	for (auto& barrel : barrels)
		barrel = new ExplodingBarrel();

}

void ExplodingBarrelsFactory::SetMaterialProperties()
{
	modelShader->Bind();
	//we are using the red channel
	modelShader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	modelShader->SetFloat("material.shininess", 32.0f);
	modelShader->Unbind();
}

void ExplodingBarrelsFactory::CreateExplosion(glm::vec3& position)
{
	for (auto& barrel : barrels) {

		if (!barrel->active){
		barrel->Init(position);
		break;
		}
	}
}

void ExplodingBarrelsFactory::Render()
{
	modelShader->Bind();
	modelShader->SetMat4x4("projection", Game::perspective);
	modelShader->SetMat4x4("view", Game::view);
	const glm::vec3 camPos = Game::GetCameraPosition();
	modelShader->SetFloat3("viewPos", camPos.x, camPos.y, camPos.z);
	for (auto& barrel : barrels) {

		if (!barrel->active)
			continue;

		barrel->Draw(*modelShader);
	}
}
void ExplodingBarrelsFactory::Update(float deltaTime)
{
	/*if(!barrels->active)
		ExplodingBarrelsFactory::CreateExplosion(glm::vec3(0, 5, -5));*/
	for (auto& barrel : barrels) {

		if (!barrel->active)
			continue;


		barrel->Update(deltaTime);
	}
}
