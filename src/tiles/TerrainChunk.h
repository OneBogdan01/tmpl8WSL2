#pragma once
#include <vector>

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
	vector<unsigned int> indices;

	float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE];
	GLuint VBO, VAO;
	const float minH = -3;
	const float maxH = 9;
	const float randomness = 9;
	TerrainSettings settings = TerrainSettings( minH, maxH, randomness);
	GLuint EBO;

};
