#pragma once
#include "utilities/DiamondSquare.h"

class TerrainChunk
{
public:
	TerrainChunk();
	void Init();
	void Draw();
	void PrintHeightMap(const float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE]) const;
	glm::vec3 position = glm::vec3(0.0f);

private:
	float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE];
	GLuint VBO, VAO;
	const int size = 25;
	const float minH = -3;
	const float maxH = 9;
	const float randomness = 9;
	TerrainSettings settings = TerrainSettings(size, minH, maxH, randomness);

};
