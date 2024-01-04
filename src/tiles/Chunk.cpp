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

btRigidBody* Chunk::AddStaticRigidbody(const char* modelId, const glm::vec3 initialPosition, glm::vec3& drawOffset)
{
	//make a collision shape
	// Create a collision shape (e.g., a box shape for a rectangular tile)
	ModelTileFactory& factory = ModelTileFactory::GetInstance();
	btBoxShape* tileShape = World::CreateBoundingBoxModel(factory.GetModel(modelId)->GetMeshes(), TILE_SIZE);

	// Create a motion state
	btTransform tileTransform;
	tileTransform.setIdentity();
	drawOffset = glm::vec3(0.0f, tileShape->getHalfExtentsWithMargin().y(), 0.0f);
	/*tileTransform.setOrigin(btVector3(initialPosition.x, initialPosition.y + drawOffset.y,
		initialPosition.z));*/
	btDefaultMotionState* tileMotionState = new btDefaultMotionState(tileTransform);

	// Create a rigid body
	btScalar mass = 0.0; // Mass 0 for static objects
	btVector3 localInertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, tileMotionState, tileShape, localInertia);

	btRigidBody* rigidBody = new btRigidBody(rbInfo);
	//set it as kinamatic
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);




	Game::world.AddRigidBody(rigidBody);
	return rigidBody;
}

void Chunk::LoadTile(const size_t index, const char* path, const glm::vec3 pos)
{
	tiles[index].Init(path, pos, index);
	if (rbTiles[index] == nullptr)
		rbTiles[index] = AddStaticRigidbody(path, pos, drawOffset);
	tiles[index].SetUserPointer(rbTiles[index]);
	activeTiles.push_back(index);
	coinLow[index] = 1;
}

void Chunk::LoadCoins(const size_t index, const char* path, const glm::vec3 pos)
{
	coins[index].Init(path, pos, index);
	coins[index].GetCallback().GetEvent().connect(&Chunk::DisableCoin, this);
}

void Chunk::LoadObstacles(const size_t index, const char* path, const glm::vec3 pos)
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
	position.z = -3.0f;

}

void Chunk::SetMaterialProperties()
{
	modelShader->Bind();
	//we are using the red channel
	modelShader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	modelShader->SetFloat("material.shininess", 32.0f);
	modelShader->Unbind();
}

//TODO  make it into the math library
float Chunk::lerp(const float v0, const float v1, const float t)
{
	return (1 - t) * v0 + t * v1;
}

float Chunk::invLerp(const float a, const float b, const float v)
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

void Chunk::DisableCoin(const size_t index)
{
	glm::vec3 pos = glm::vec3(60.0f);
	//offscreen
	coins[index].UpdatePhysicsPosition(pos);
	activeCoins.erase(std::remove(activeCoins.begin(), activeCoins.end(), index), activeCoins.end());
}

void Chunk::DisableObstacle(const size_t index)
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
				coins[indexCoin].offset = (glm::vec3(0.0f, perlinVal, 0.0f));

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
		glm::vec3 tilePosition = tiles[index].GetPosition(position);
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


void Chunk::UpdateRB()
{
	for (auto& index : activeTiles)
	{
		//tiles[index].UpdatePhysicsPosition(position);
		btRigidBody* body = rbTiles[index];
		btTransform newTransform;
		body->getMotionState()->getWorldTransform(newTransform);
		glm::vec3 tilePos = tiles[index].initialPosition;
		tilePos.y += drawOffset.y;
		newTransform.setOrigin(btVector3(GlmToBtVector3(position + tilePos)));
		body->getMotionState()->setWorldTransform(newTransform);
	}
}

void Chunk::Update(float deltaTime)
{
	//UpdateRB();
	for (auto& index : activeCoins)
	{
		coins[index].UpdatePhysicsPosition(position);
	}
	for (auto& index : activeObstacles)
	{
		obstacles[index].UpdatePhysicsPosition(position);
	}
}

void Chunk::SetPosition(const glm::vec3 pos)
{
	position = pos;
}

void Chunk::Translate(const glm::vec3 pos)
{
	position += pos;
}

glm::vec3 Chunk::GetPosition()
{
	return position;
}
