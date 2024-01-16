#pragma once
#include "RandomNumberGenerator.h"
constexpr int MAX_TERRAIN_SIZE = 129;

//based on https://www.youtube.com/watch?v=4GuAV1PnurU&list=WL&index=2
struct TerrainSettings
{
	int size;
	float minHeight;
	float maxHeight;
	float randomness;
	//generate cunostructor
	TerrainSettings(int size, float minHeight, float maxHeight, float randomness) :
		size(size), minHeight(minHeight), maxHeight(maxHeight), randomness(randomness)
	{
	}
};

class DiamondSquare
{
public:
	static void generateHeightMap(const TerrainSettings& settings, float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE])
	{
		int size = settings.size;
		int minH = settings.minHeight;
		int maxH = settings.maxHeight;

		heightMap[0][0] = RandomNumberGenerator::RandomFloat();
		heightMap[size - 1][0] = RandomNumberGenerator::RandomFloat();
		heightMap[0][size - 1] = RandomNumberGenerator::RandomFloat();
		heightMap[size - 1][size - 1] = RandomNumberGenerator::RandomFloat();
	}
};
