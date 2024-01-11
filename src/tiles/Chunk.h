#pragma once
#include <array>
#include <unordered_set>
#include <vector>

#include "Tile.h"

#include "Rope.h"
#include "CoinTile.h"
#include "GroundTile.h"
#include "ObstacleTile.h"


constexpr unsigned int TILES_PER_CHUNK = 18;
constexpr unsigned int COINS_PER_CHUNK = 9;
constexpr unsigned int OBSTACLES_PER_CHUNK = 6;
constexpr unsigned int ROPES_PER_CHUNK = 1;


class Chunk
{
public:
	~Chunk();
	void LoadTile(size_t index, const char* path, glm::vec3 pos);
	void LoadCoins(size_t index, const char* path, glm::vec3 pos);
	void LoadRopes(size_t index, const char* path, glm::vec3 pos);
	void LoadObstacles(size_t index, const char* path, glm::vec3 pos);
	void LoadProps(const char* coinPath, const char* obstaclePath);
	void ResetTiles();
	void ResetRopes();
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
	void DisableRope(size_t index);

private:
	void SetMaterialProperties();
	//pools of rigid bodies and triggers
	//TODO do the same for the triggers
	inline static std::array<btRigidBody*, TILES_PER_CHUNK> rbTiles;

	inline static glm::vec3 drawOffset;
	//ground layer
	std::array<GroundTile, TILES_PER_CHUNK> tiles;
	std::unordered_set<const char*> tileIDs;
	std::array<int, TILES_PER_CHUNK> coinLow = {0};
	std::array<CoinTile, COINS_PER_CHUNK> coins;
	std::unordered_set<const char*> coinIDs;

	std::array<ObstacleTile, OBSTACLES_PER_CHUNK> obstacles;
	std::unordered_set<const char*> obstaclesIDs;

	std::array<Rope, ROPES_PER_CHUNK> ropes;


	std::vector<size_t> activeTiles;
	std::vector<size_t> activeCoins;
	std::vector<size_t> activeObstacles;
	std::vector<size_t> activeRopes;
	glm::vec3 position = glm::vec3(0.0f);
	Shader* modelShader = nullptr;
};
