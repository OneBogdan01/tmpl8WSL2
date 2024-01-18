#pragma once

#include "utilities/DiamondSquare.h"

class TerrainChunk
{
public:
	TerrainChunk();
	glm::vec3 GetColor(float height);
	void Init();
	void Draw();
	void PrintHeightMap(const float _heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE]) const;
	glm::vec3 position = glm::vec3(-MAX_TERRAIN_SIZE , -6.f, 0);
	void SetLastRow();
private:
	static inline float firstRow[MAX_TERRAIN_SIZE] = { -1 };
	static inline float lastRow[MAX_TERRAIN_SIZE] = { -1 };
	vector<unsigned int> indices;

	float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE] = { {0.1f} };
	GLuint VBO, VAO;
	const float minH = 1.3f;
	const float maxH = 12.6f;
	const float randomness = 5.0f;

	TerrainSettings settings = TerrainSettings(minH, maxH, randomness);
	GLuint EBO;


};
