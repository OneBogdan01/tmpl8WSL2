#pragma once
#include <array>
#include <vector>


#include "GroundTile.h"
#include "Tile.h"
constexpr unsigned int TILES_PER_CHUNK = 18;


class Chunk
{
public:
	~Chunk();
	void LoadTile(size_t index, const char* path, glm::vec3 pos);
	void ResetTiles();
	Chunk();

	void Draw();
	void Update(float deltaTime);
	void SetPosition(glm::vec3 pos);

	void Translate(glm::vec3 pos);

	glm::vec3 GetPosition();

private:
	void SetMaterialProperties();

	//ground layer
	std::array<GroundTile, TILES_PER_CHUNK> tiles;
	std::vector<size_t> activeTiles;
	glm::vec3 position = glm::vec3(0.0f);
	Shader* modelShader = nullptr;
};
