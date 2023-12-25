#pragma once
#include <array>
#include <vector>


#include "CoinTile.h"
#include "GroundTile.h"
#include "ObstacleTile.h"
#include "Tile.h"




constexpr unsigned int TILES_PER_CHUNK = 18;
constexpr unsigned int COINS_PER_CHUNK = 9;
constexpr unsigned int OBSTACLES_PER_CHUNK = 6;


class Chunk
{
public:
	~Chunk();
	void LoadTile(size_t index, const char* path, glm::vec3 pos);
	void LoadCoins(size_t index, const char* path, glm::vec3 pos);
	void LoadObstacles(size_t index, const char* path, glm::vec3 pos);
	void LoadProps(const char* coinPath, const char* obstaclePath);
	void ResetTiles();
	Chunk();
	btRigidBody* AddStaticRigidbody(const char* modelId, glm::vec3 initialPosition, glm::vec3& drawOffset);

	void Draw();
	void UpdateRB();
	void Update(float deltaTime);
	void SetPosition(glm::vec3 pos);

	void Translate(glm::vec3 pos);

	glm::vec3 GetPosition();
	void RandomizeChunk();
	void HideChunk();
	void DisableCoin(size_t index);
	void DisableObstacle(size_t index);

private:
	float lerp(float v0, float v1, float t);
	float invLerp(float a, float b, float v);
	void SetMaterialProperties();
	//pools of rigid bodies and triggers
	//TODO do the same for the triggers
	inline static std::array<btRigidBody*, TILES_PER_CHUNK> rbTiles;

	inline static glm::vec3 drawOffset;
	//ground layer
	std::array<GroundTile, TILES_PER_CHUNK> tiles;
	std::array<int, TILES_PER_CHUNK> coinLow = { 0 };
	std::array<CoinTile, COINS_PER_CHUNK > coins;
	std::array<ObstacleTile, OBSTACLES_PER_CHUNK > obstacles;
	std::vector<size_t> activeTiles;
	std::vector<size_t> activeCoins;
	std::vector<size_t> activeObstacles;
	glm::vec3 position = glm::vec3(0.0f);
	Shader* modelShader = nullptr;
};
