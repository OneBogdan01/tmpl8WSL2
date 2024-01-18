#pragma once
#include "RandomNumberGenerator.h"
constexpr int MAX_TERRAIN_SIZE = 33;
constexpr float MIN_THRESHOLD = 0.2f;

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
using  rng = RandomNumberGenerator;
class DiamondSquare
{
public:
	static float RandomHeightValue(float roughness)
	{
		float sign = rng::RandomFloat() > 0.5f ? 1.0f : -1.0f;
		return rng::RandomFloat() * roughness * sign;
	}

	static void generateHeightMap(const TerrainSettings& settings, float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE], float& maxHeight)
	{
		maxHeight = -1;
		const float minH = settings.minHeight;
		const float maxH = settings.maxHeight;


		float roughness = settings.randomness;
		rng::RandomInRange(minH, maxH);
		heightMap[0][0] = RandomHeightValue(roughness) + rng::RandomInRange(minH, maxH);
		if (maxHeight < heightMap[0][0])
			maxHeight = heightMap[0][0];
		heightMap[MAX_TERRAIN_SIZE - 1][0] = RandomHeightValue(roughness) + rng::RandomInRange(minH, maxH);
		if (maxHeight < heightMap[MAX_TERRAIN_SIZE - 1][0])
			maxHeight = heightMap[MAX_TERRAIN_SIZE - 1][0];
		heightMap[0][MAX_TERRAIN_SIZE - 1] = RandomHeightValue(roughness) + rng::RandomInRange(minH, maxH);
		if (maxHeight < heightMap[0][MAX_TERRAIN_SIZE - 1])
			maxHeight = heightMap[0][MAX_TERRAIN_SIZE - 1];
		heightMap[MAX_TERRAIN_SIZE - 1][MAX_TERRAIN_SIZE - 1] = RandomHeightValue(roughness) + rng::RandomInRange(minH, maxH);
		if (maxHeight < heightMap[MAX_TERRAIN_SIZE - 1][MAX_TERRAIN_SIZE - 1])
			maxHeight = heightMap[MAX_TERRAIN_SIZE - 1][MAX_TERRAIN_SIZE - 1];
		int chunkSize = MAX_TERRAIN_SIZE - 1;
		while (chunkSize > 1)
		{
			int half = chunkSize / 2;
			//square step
			for (int i = 0; i < MAX_TERRAIN_SIZE - 1; i += chunkSize)
			{
				for (int j = 0; j < MAX_TERRAIN_SIZE - 1; j += chunkSize)
				{
					float avg = 0;
					avg += heightMap[i][j];
					avg += heightMap[i + chunkSize][j];
					avg += heightMap[i][j + chunkSize];
					avg += heightMap[i + chunkSize][j + chunkSize];
					avg /= 4;
					if (heightMap[i + half][j + half] > MIN_THRESHOLD)
						continue;
					heightMap[i + half][j + half] = avg + RandomHeightValue(roughness);

					if (0.3f > heightMap[i + half][j + half])
						heightMap[i + half][j + half] = .3f;
					//heightMap[i + half][j] = avg + rng::RandomFloat() * roughness;
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
					if (heightMap[i][j] > MIN_THRESHOLD)
						continue;
					heightMap[i][j] = avg + RandomHeightValue(roughness);
					if (0.3f > heightMap[i][j])
						heightMap[i][j] = .3f;
				}
			}

			chunkSize /= 2;
			roughness /= 2;
		}
	}
	static void generateHeightMapNoFirstRow(const TerrainSettings& settings, float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE], float& maxHeight)
	{
		maxHeight = -1;
		const float minH = settings.minHeight;
		const float maxH = settings.maxHeight;


		float roughness = settings.randomness;

		/*heightMap[0][0] = RandomHeightValue(minH, maxH, roughness);
		if (maxHeight < heightMap[0][0])
			maxHeight = heightMap[0][0];*/
			/*heightMap[MAX_TERRAIN_SIZE - 1][0] = RandomHeightValue(minH, maxH, roughness);
			if (maxHeight < heightMap[MAX_TERRAIN_SIZE - 1][0])
				maxHeight = heightMap[MAX_TERRAIN_SIZE - 1][0];*/
		if (heightMap[MAX_TERRAIN_SIZE - 1][0] < MIN_THRESHOLD) {
			heightMap[MAX_TERRAIN_SIZE - 1][0] = RandomHeightValue(roughness) + rng::RandomInRange(minH, maxH);
			if (maxHeight < heightMap[0][MAX_TERRAIN_SIZE - 1])
				maxHeight = heightMap[0][MAX_TERRAIN_SIZE - 1];
		}
		if (heightMap[MAX_TERRAIN_SIZE - 1][MAX_TERRAIN_SIZE - 1] < MIN_THRESHOLD) {
			heightMap[MAX_TERRAIN_SIZE - 1][MAX_TERRAIN_SIZE - 1] = RandomHeightValue(roughness) + rng::RandomInRange(minH, maxH);
			if (maxHeight < heightMap[MAX_TERRAIN_SIZE - 1][MAX_TERRAIN_SIZE - 1])
				maxHeight = heightMap[MAX_TERRAIN_SIZE - 1][MAX_TERRAIN_SIZE - 1];
		}
		int chunkSize = MAX_TERRAIN_SIZE - 1;
		while (chunkSize > 1)
		{
			int half = chunkSize / 2;
			//square step
			for (int i = 0; i < MAX_TERRAIN_SIZE - 1; i += chunkSize)
			{
				for (int j = 0; j < MAX_TERRAIN_SIZE - 1; j += chunkSize)
				{
					float avg = 0;
					avg += heightMap[i][j];
					avg += heightMap[i + chunkSize][j];
					avg += heightMap[i][j + chunkSize];
					avg += heightMap[i + chunkSize][j + chunkSize];
					if (heightMap[i + half][j + half] > MIN_THRESHOLD)
						continue;
					avg /= 4;
					heightMap[i + half][j + half] = avg + RandomHeightValue(roughness);
					if (0.3f > heightMap[i + half][j + half])
						heightMap[i + half][j + half] = .3f;
					//heightMap[i + half][j] = avg + rng::RandomFloat() * roughness;
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
					if (heightMap[i][j] > MIN_THRESHOLD)
						continue;
					avg /= count;
					heightMap[i][j] = avg + RandomHeightValue(roughness);
					if (0.3f > heightMap[i][j])
						heightMap[i ][j ] = .3f;
				}
			}

			chunkSize /= 2;
			roughness /= 2;
		}
	}
};
