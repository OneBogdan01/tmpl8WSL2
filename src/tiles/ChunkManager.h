#pragma once
#include "Chunk.h"
#include "template.h"

constexpr float TILE_SIZE = 5.0f;
constexpr float OFFSET_TILE_MULTIPLIER = -1.1f;
constexpr size_t NUMBER_OF_CHUNKS = 9;
constexpr uint NUMBER_OF_ACTIVE_CHUNKS = 3;
constexpr uint COINS_INDEX = 10;
constexpr uint OBSTACLE_INDEX = 8;
constexpr uint OBSTACLE_DESTRUCTIBLE_INDEX = 9;

//inspired by my old class from block A https://github.com/Tycro-Games/BlockA-Pitfall/blob/master/Scripts/Map/Tilemap.cpp
class ChunkManager
{
public:
	~ChunkManager();

	void Init();
	void DrawChunks();
	void Update(float deltaTime);
	void SetDirection(glm::vec3 _dir);

	static uint GetHeight()
	{
		return heightY;
	}

	static uint GetWidth()
	{
		return widthX;
	}

private:
	std::array<Chunk*, NUMBER_OF_CHUNKS> chunks = {};

	//helper functions for parsing
	void LoadCSVFile(const char* csvPath);
	bool hasEnding(const std::string& fullString, const std::string& ending);
	void ConvertCharToInt(const char* pch, uint& numberForm);
	void ExtractWidthHeight(const char* csvRaw, uint& w, uint& h);
	std::vector<std::string> tilePaths;

	static inline uint widthX = 0;
	static inline uint heightY = 0;
	uint size = 0;
	uint* tileArray = nullptr;
	glm::vec3 dir = {0.0f, 0.0f, 15.0f};
	glm::vec3 chunkOffset = glm::vec3(0.0f);
};
