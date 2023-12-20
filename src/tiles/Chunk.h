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
	void SetMaterialProperties();
	Chunk();

	//Chunk& operator=(const Chunk& other)
	//{
	//	if (this != &other) // protect against invalid self-assignment
	//	{
	//		// 1: deallocate old memory

	//		// 2: allocate new memory and copy the elements
	//		tiles = other.tiles;
	//		activeTiles = other.activeTiles;
	//		position = other.position;
	//		modelShader = other.modelShader; // assuming Shader has a valid copy assignment operator

	//		// 3: transfer ownership if needed
	//	}
	//	return *this;
	//}

	//Chunk(Chunk&& other) noexcept
	//{
	//	// Move data from 'other' object to 'this' object
	//	tiles = std::move(other.tiles);
	//	activeTiles = std::move(other.activeTiles);
	//	position = std::move(other.position);
	//	modelShader = other.modelShader;

	//	// Nullify the data in 'other'
	//	other.modelShader = nullptr;
	//}

	//void ResetOffsetPosition();
	// Copy constructor

	void Draw();
	void Update(float deltaTime);
	void SetPosition(glm::vec3 pos);

	void Translate(glm::vec3 pos);

	glm::vec3 GetPosition();

private:
	//ground layer
	std::array<GroundTile, TILES_PER_CHUNK> tiles;
	std::vector<size_t> activeTiles;
	glm::vec3 position = glm::vec3(0.0f);
	Shader* modelShader = nullptr;
};
