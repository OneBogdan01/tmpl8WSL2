#include "Chunk.h"

#include <iostream>


#include "Camera.h"
#include "Camera.h"
#include "game.h"
#include "GroundTileFactory.h"
#include "ChunkManager.h"
#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"

#include "utilities/RandomNumberGenerator.h"

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

void Chunk::LoadCoins(size_t index, const char* path, glm::vec3 pos)
{
	coins[index].Init(path, pos);
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

//to make it into the math library
float Chunk::lerp(float v0, float v1, float t)
{
	return (1 - t) * v0 + t * v1;
}

float Chunk::invLerp(float a, float b, float v)
{
	return (v - a) / (b - a);
}

void Chunk::HideChunk()
{
	glm::vec3 pos = glm::vec3(60.0f);

	for (auto& coin : coins)
	{
		coin.UpdatePhysicsPosition(pos);
	}
}

void Chunk::RandomizeChunk()
{
	HideChunk();
	const float h = static_cast<float>(ChunkManager::GetHeight());
	const float w = static_cast<float>(ChunkManager::GetWidth());
	RandomNumberGenerator::RandomizePerlinNoise();
	float threshold = 0.75f - RandomNumberGenerator::RandomFloat() * 0.5f;
	int count = static_cast<int>(lerp(1.0f, TILES_PER_CHUNK / 2, RandomNumberGenerator::RandomFloat()));
	cout << threshold << endl;
	for (uint i = 0; i < h; i++)
	{
		for (uint j = 0; j < w; j++)
		{
			if (count == 0)
				break;
			uint index = j + i * w;
			float x = invLerp(0.0f, h, static_cast<float>(i));
			float y = invLerp(0.0f, w, static_cast<float>(j));
			cout << RandomNumberGenerator::noise2D(x, y) << " ";
			if (abs(RandomNumberGenerator::noise2D(x, y)) > threshold)
			{
				count--;
				coins[index].ResetPosition();
				activeCoins.push_back(index);
			}
			else
			{
				threshold -= 0.025f;
			}
		}
		cout << endl;
	}
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
	//draw ground
	for (auto& index : activeTiles)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 tilePosition = tiles[index].GetPosition();
		model = glm::translate(model, tilePosition);
		model = glm::scale(model, glm::vec3(TILE_SIZE));
		modelShader->SetMat4x4("model", model);
		groundFactory.Draw(tiles[index].GetId(), *modelShader);
	}
	// Draw coins
	for (auto& index : activeCoins)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 coinPosition = coins[index].GetPosition();

		model = glm::translate(model, coinPosition);
		model = glm::scale(model, glm::vec3(TILE_SIZE));
		modelShader->SetMat4x4("model", model);
		groundFactory.Draw(coins[index].GetId(), *modelShader);
	}
	modelShader->Unbind();
}

void Chunk::Update(float deltaTime)
{
	for (auto& index : activeTiles)
	{
		tiles[index].UpdatePhysicsPosition(position);
	}
	for (auto& index : activeCoins)
	{
		coins[index].UpdatePhysicsPosition(position);
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
