#include "Chunk.h"

#include <iostream>


#include "Camera.h"
#include "game.h"
#include "ModelTileFactory.h"
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
	Game::lightManager->SetLightProperties(*modelShader);
}

void Chunk::LoadTile(size_t index, const char* path, glm::vec3 pos)
{
	tiles[index].Init(path, pos, index);

	activeTiles.push_back(index);
	coinLow[index] = 1;
}

void Chunk::LoadCoins(size_t index, const char* path, glm::vec3 pos)
{
	coins[index].Init(path, pos, index);
	coins[index].GetCallback().GetEvent().connect(&Chunk::DisableCoin, this);
}

void Chunk::LoadObstacles(size_t index, const char* path, glm::vec3 pos)
{
	obstacles[index].Init(path, pos, index);
	obstacles[index].GetCallback().GetEvent().connect(&Chunk::DisableObstacle, this);
}
void Chunk::LoadProps(const char* coinPath, const char* obstaclePath)
{
	for (uint index = 0; index < COINS_PER_CHUNK; index++)
	{
		LoadCoins(index, coinPath, glm::vec3(0));
	}
	for (uint index = 0; index < OBSTACLES_PER_CHUNK; index++)
	{
		LoadObstacles(index, obstaclePath, glm::vec3(0));
	}
}
void Chunk::ResetTiles()
{
	position.z = 0;
}

void Chunk::SetMaterialProperties()
{
	modelShader->Bind();
	//we are using the red channel
	modelShader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	modelShader->SetFloat("material.shininess", 32.0f);
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
	for (auto& obstacle : obstacles)
	{
		obstacle.UpdatePhysicsPosition(pos);
	}
	activeCoins.clear();
	activeObstacles.clear();
}

void Chunk::DisableCoin(size_t index)
{
	glm::vec3 pos = glm::vec3(60.0f);
	//offscreen
	coins[index].UpdatePhysicsPosition(pos);
	activeCoins.erase(std::remove(activeCoins.begin(), activeCoins.end(), index), activeCoins.end());
}

void Chunk::DisableObstacle(size_t index)
{
	glm::vec3 pos = glm::vec3(60.0f);
	//offscreen
	obstacles[index].UpdatePhysicsPosition(pos);
	activeObstacles.erase(std::remove(activeObstacles.begin(), activeObstacles.end(), index), activeObstacles.end());
}

void Chunk::RandomizeChunk()
{
	HideChunk();
	const float h = static_cast<float>(ChunkManager::GetHeight());
	const float w = static_cast<float>(ChunkManager::GetWidth());
	RandomNumberGenerator::RandomizePerlinNoise();
	float threshold = RandomNumberGenerator::RandomFloat() * 0.25f + .5f;
	float rng = RandomNumberGenerator::RandomFloat() * 0.4f + .2f;
	uint maxCoinCount = static_cast<float>(COINS_PER_CHUNK) * RandomNumberGenerator::RandomFloat();
	cout << "Count:" << maxCoinCount << endl;
	float x = RandomNumberGenerator::RandomFloat() * 512;
	float y = RandomNumberGenerator::RandomFloat() * 512;
	int indexObstacleOccupied[TILES_PER_CHUNK] = { 0 };
	//obstacles
	uint maxObstacleCount = static_cast<uint>(RandomNumberGenerator::RandomFloat() * OBSTACLES_PER_CHUNK);
	for (uint i = 0; i < h && maxObstacleCount > 0; i++)
	{
		for (uint j = 0; j < w && maxObstacleCount > 0; j++)
		{
			int index = j + i * w;
			int nextIndex = std::clamp(index + static_cast<int>(w), 0, static_cast<int>(TILES_PER_CHUNK - 1));
			int prevIndex = std::clamp(index - static_cast<int>(w), 0, static_cast<int>(TILES_PER_CHUNK - 1));
			/*&& tiles[nextIndex].GetId() != nullptr &&
				tiles[prevIndex].GetId() != nullptr*/
			if (tiles[index].GetId() != nullptr)
				if (RandomNumberGenerator::RandomFloat() > .85f)
				{
					maxObstacleCount--;
					size_t indexObstacle = activeObstacles.size();
					glm::vec3 obstaclePos = tiles[index].initialPosition;
					obstaclePos.y = 0;
					obstacles[indexObstacle].initialPosition = obstaclePos;

					obstacles[indexObstacle].ResetPosition();
					activeObstacles.push_back(indexObstacle);
					//go to next column at least
					indexObstacleOccupied[index] = 1;
					i += 2;
					if (i >= h)
					{
						maxObstacleCount = 0;
					}
				}
		}
	}

	//coins
	for (uint i = 0; i < h && maxCoinCount > 0; i++)
	{
		for (uint j = 0; j < w && maxCoinCount > 0; j++)
		{
			uint index = j + i * w;

			x += static_cast<float>(i);
			y += static_cast<float>(j);
			float aux = RandomNumberGenerator::noise2D(y, x);
			if (aux < 0)
				aux *= -1.0f;

			float perlinVal = aux;;


			while (perlinVal < 1.0f)
			{
				perlinVal *= 3.0f;
			}
			perlinVal = std::clamp(perlinVal, 0.0f, TILE_SIZE / 2);

			glm::vec3 coinPos = tiles[index].initialPosition;
			coinPos.y += TILE_SIZE;

			//it is a high coin
			if (coinLow[index] == 0)
			{
				perlinVal *= -1;
				coinPos.y += TILE_SIZE;
			}
			//low coin, but we placed obstacle
			else if (indexObstacleOccupied[index] == 1)
			{
				perlinVal += TILE_SIZE;

			}
			cout << perlinVal << " ";

			if (rng > threshold)
			{
				maxCoinCount--;
				size_t indexCoin = activeCoins.size();
				coins[indexCoin].SetOffset(glm::vec3(0.0f, perlinVal, 0.0f));

				coins[indexCoin].initialPosition = coinPos;
				coins[indexCoin].ResetPosition();
				activeCoins.push_back(indexCoin);
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
	const auto& groundFactory = ModelTileFactory::GetInstance();


	modelShader->Bind();
	modelShader->SetMat4x4("projection", Game::perspective);
	modelShader->SetMat4x4("view", Game::view);
	const glm::vec3 camPos = Game::GetCameraPosition();
	modelShader->SetFloat3("viewPos", camPos.x, camPos.y, camPos.z);
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
	for (auto& index : activeObstacles)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 obstaclePosition = obstacles[index].GetPosition();

		model = glm::translate(model, obstaclePosition);
		model = glm::scale(model, glm::vec3(TILE_SIZE * 2));
		modelShader->SetMat4x4("model", model);
		groundFactory.Draw(obstacles[index].GetId(), *modelShader);
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
	for (auto& index : activeObstacles)
	{
		obstacles[index].UpdatePhysicsPosition(position);
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
