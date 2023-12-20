#include "Chunk.h"


#include "game.h"
#include "GroundTileFactory.h"
#include "ChunkManager.h"
#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"

Chunk::~Chunk()
{
	delete modelShader;
}

Chunk::Chunk()
{
	modelShader = new Shader(
		"assets/shaders/ModelLoading.vert",
		"assets/shaders/ModelLoading.frag");
	SetMaterialProperties();
}

void Chunk::LoadTile(size_t index, const char* path, glm::vec3 pos)
{
	tiles[index].Init(path, pos);
	activeTiles.push_back(index);
}

void Chunk::ResetTiles()
{
	position.z = 0;
}

void Chunk::SetMaterialProperties()
{
	modelShader->Bind();
	modelShader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	modelShader->SetFloat("material.shininess", 32.0f);
	modelShader->SetFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	modelShader->SetFloat3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
	modelShader->SetFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	modelShader->Unbind();
}

void Chunk::Draw()
{
	auto groundFactory = GroundTileFactory::GetInstance();


	modelShader->Bind();
	modelShader->SetMat4x4("projection", Game::perspective);
	modelShader->SetMat4x4("view", Game::view);
	const glm::vec3 camPos = Game::GetCameraPosition();
	modelShader->SetFloat3("viewPos", camPos.x, camPos.y, camPos.z);
	modelShader->SetFloat3("lightPos", Game::GetLightPos().x, Game::GetLightPos().y, Game::GetLightPos().z);
	for (auto& index : activeTiles)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 tilePosition = tiles[index].GetPosition();
		model = glm::translate(model, tilePosition);
		model = glm::scale(model, glm::vec3(TILE_SIZE));
		modelShader->SetMat4x4("model", model);

		groundFactory.Draw(tiles[index].GetId(), *modelShader);
	}

	modelShader->Unbind();
}

void Chunk::Update(float deltaTime)
{
	for (auto& index : activeTiles)
	{
		tiles[index].UpdatePhysicsPosition(position);
	}
}

void Chunk::SetPosition(glm::vec3 pos)
{
	position = pos;
}

void Chunk::Translate(glm::vec3 pos)
{
	position += pos;
}

glm::vec3 Chunk::GetPosition()
{
	return position;
}
