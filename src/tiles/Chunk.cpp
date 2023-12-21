#include "Chunk.h"

#include <iostream>


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
	tiles[index].Init(path, pos, index);

	activeTiles.push_back(index);
}

void Chunk::LoadCoins(size_t index, const char* path, glm::vec3 pos)
{
	coins[index].Init(path, pos, index);
	coins[index].GetCallback().GetEvent().connect(&Chunk::DisableCoin, this);
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
	activeCoins.clear();
}

void Chunk::DisableCoin(size_t index)
{
	glm::vec3 pos = glm::vec3(60.0f);
	//offscreen
	coins[index].UpdatePhysicsPosition(pos);
	activeCoins.erase(std::remove(activeCoins.begin(), activeCoins.end(), index), activeCoins.end());
}

void Chunk::RandomizeChunk()
{
	HideChunk();
	const float h = static_cast<float>(ChunkManager::GetHeight());
	const float w = static_cast<float>(ChunkManager::GetWidth());
	RandomNumberGenerator::RandomizePerlinNoise();
	float threshold = RandomNumberGenerator::RandomFloat() * 0.25f + .5f;
	float rng = RandomNumberGenerator::RandomFloat() * 0.4f + .2f;
	int count =  static_cast<float>(TILES_PER_CHUNK) / 2 * RandomNumberGenerator::RandomFloat();
	cout << "Count:" << count << endl;
	float x = RandomNumberGenerator::RandomFloat() * 512;
	float y = RandomNumberGenerator::RandomFloat() * 512;
	for (uint i = 0; i < h && count > 0; i++)
	{
		for (uint j = 0; j < w&& count > 0; j++)
		{

			uint index = j + i * w;
			x += static_cast<float>(i);
			y += static_cast<float>(j);
			float aux = RandomNumberGenerator::noise2D(y, x);
			if (aux < 0)
				aux *= -1.0f;

			float perlinVal = aux; ;


			while (perlinVal < 1.0f)
			{
				perlinVal *= 3.0f;
			}
			perlinVal = std::clamp(perlinVal, 0.0f, TILE_SIZE/2);


			if (coins[index].GetTileInitPosition().y() > TILE_SIZE / 2.0f)
			{
				perlinVal *= -1;
			}
			cout << perlinVal << " ";

			if (rng > threshold)
			{
				count--;
				coins[index].SetOffset(glm::vec3(0.0f, perlinVal, 0.0f));


				coins[index].ResetPosition();
				activeCoins.push_back(index);
			}
			else
			{
				threshold -= RandomNumberGenerator::RandomFloat() * 0.1f;
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

void DisableCoin(void)
{
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
