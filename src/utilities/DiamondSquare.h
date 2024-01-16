#pragma once
#include "RandomNumberGenerator.h"
constexpr int MAX_TERRAIN_SIZE = 9;

//based on https://www.youtube.com/watch?v=4GuAV1PnurU&list=WL&index=2
struct TerrainSettings
{
	float minHeight;
	float maxHeight;
	float randomness;
	//generate cunostructor
	TerrainSettings(float minHeight, float maxHeight, float randomness) :
		minHeight(minHeight), maxHeight(maxHeight), randomness(randomness)
	{
	}
};

class DiamondSquare
{
public:
	static void generateHeightMap(const TerrainSettings& settings, float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE])
	{
		int minH = settings.minHeight;
		int maxH = settings.maxHeight;

		heightMap[0][0] = RandomNumberGenerator::RandomFloat();
		heightMap[MAX_TERRAIN_SIZE - 1][0] = RandomNumberGenerator::RandomFloat();
		heightMap[0][MAX_TERRAIN_SIZE - 1] = RandomNumberGenerator::RandomFloat();
		heightMap[MAX_TERRAIN_SIZE - 1][MAX_TERRAIN_SIZE - 1] = RandomNumberGenerator::RandomFloat();
		float roughness = 2;
		int chunkSize = MAX_TERRAIN_SIZE - 1;
		while (chunkSize > 1)
		{
			int half = chunkSize / 2;
			//square step
			for (int i = 0; i < MAX_TERRAIN_SIZE-1; i += chunkSize)
			{
				for (int j = 0; j < MAX_TERRAIN_SIZE-1; j += chunkSize)
				{
					float avg = 0;
					avg += heightMap[i][j];
					avg += heightMap[i + chunkSize][j];
					avg += heightMap[i][j+ chunkSize];
					avg += heightMap[i+ chunkSize][j+ chunkSize];
					avg /= 4;
					heightMap[i + half][j + half] = avg + RandomNumberGenerator::RandomFloat() * roughness;
					//heightMap[i + half][j] = avg + RandomNumberGenerator::RandomFloat() * roughness;
				}
			}
			//diamond step
			for (int i = 0; i < MAX_TERRAIN_SIZE; i += half)
			{
				for (int j = (i + half) % chunkSize; j < MAX_TERRAIN_SIZE; j += chunkSize)
				{
					float avg = 0;
					int count = 0;
					if (i - half >= 0)
					{
						avg += heightMap[i - half][j];
						count++;
					}
					if (i + half < MAX_TERRAIN_SIZE)
					{
						avg += heightMap[i + half][j];
						count++;
					}
					if (j - half >= 0)
					{
						avg += heightMap[i][j - half];
						count++;
					}
					if (j + half < MAX_TERRAIN_SIZE)
					{
						avg += heightMap[i][j + half];
						count++;
					}
					avg /= count;
					heightMap[i][j] = avg + RandomNumberGenerator::RandomFloat() * roughness;
				}
			}

			chunkSize /= 2;
			roughness /= 2;
		}
	}
};
