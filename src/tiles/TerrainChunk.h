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
	glm::vec3 position = glm::vec3(-7.5f,0,-30.0f+2.5f);
	void SetLastRow();
private:
	static inline float firstRow[MAX_TERRAIN_SIZE]={-1};
	static inline float lastRow[MAX_TERRAIN_SIZE]={-1};
	vector<unsigned int> indices;

	float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE]={{0.1f}};
	GLuint VBO, VAO;
	const float minH = 1;
	const float maxH = 3;
	const float randomness = -1;
	TerrainSettings settings = TerrainSettings( minH, maxH, randomness);
	GLuint EBO;

};
