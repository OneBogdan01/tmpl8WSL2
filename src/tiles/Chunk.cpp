#include "Chunk.h"


#include "game.h"
#include "TileLoader.h"
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
}

void Chunk::LoadTile(size_t index, const char* path, glm::vec3 pos)
{
	tiles[index].Init(path, pos);
	activeTiles.push_back(index);
}

void Chunk::ResetTiles()
{
	position.z = 0;
	offset.z = 0;
	for (auto& index : activeTiles)
	{
		tiles[index].ResetPosition(glm::vec3(0.0f));
	}
}

void Chunk::Draw()
{
	//m_model = glm::scale(m_model, glm::vec3(scale, scale, scale));

	modelShader->Bind();
	modelShader->SetMat4x4("projection", Game::perspective);
	modelShader->SetMat4x4("view", Game::view);
	glm::vec3 camPos = Game::GetCameraPosition();
	modelShader->SetFloat3("viewPos", camPos.x, camPos.y, camPos.z);
	modelShader->SetFloat3("lightPos", Game::GetLightPos().x, Game::GetLightPos().y, Game::GetLightPos().z);
	modelShader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	modelShader->SetFloat("material.shininess", 32.0f);
	modelShader->SetFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	modelShader->SetFloat3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
	modelShader->SetFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	for (auto& index : activeTiles)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 position = tiles[index].GetPosition();
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(TILE_SIZE));
		modelShader->SetMat4x4("model", model);
		tiles[index].Draw(*modelShader);
	}

	modelShader->Unbind();
}

void Chunk::Update(float deltaTime)
{
	for (auto& index : activeTiles)
	{
		tiles[index].Translate(offset);
	}
	offset = glm::vec3(0.0f);
}

void Chunk::SetPosition(glm::vec3 pos)
{
	position = pos;
}

void Chunk::Translate(glm::vec3 pos)
{
	offset = pos;
	position += offset;
}

glm::vec3 Chunk::GetPosition()
{
	return position;
}
