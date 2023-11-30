#pragma once
#include <array>

#include "template.h"

#include "Tile.h"
constexpr unsigned int TILES_PER_CHUNK = 25;


class ChunkOfTiles
{
public:
	~ChunkOfTiles();
	void LoadTile(size_t index, const char* path, glm::vec3 pos);
	ChunkOfTiles();

	void Draw();

private:
	std::array<Tile, TILES_PER_CHUNK> tiles;
	Shader* modelShader = nullptr;
};
