#pragma once
#include "Chunk.h"
#include "model_loading/ogldev_types.h"

constexpr float TILE_SIZE = 5.0f;
constexpr size_t NUMBER_OF_CHUNKS = 9;
constexpr uint NUMBER_OF_ACTIVE_CHUNKS = 3;

//inspired by my old class from block A https://github.com/Tycro-Games/BlockA-Pitfall/blob/master/Scripts/Map/Tilemap.cpp
class ChunkManager
{
public:
	~ChunkManager();

	void Init();
	void DrawChunks();
	void Update(float deltaTime);
	void SetDirection(glm::vec3 _dir);

private:
	std::array<Chunk*, NUMBER_OF_CHUNKS> chunks = {};

	//helper functions for parsing
	void LoadCSVFile(const char* csvPath);
	bool hasEnding(const std::string& fullString, const std::string& ending);
	void ConvertCharToInt(const char* pch, uint& numberForm);
	void ExtractWidthHeight(const char* csvRaw, uint& w, uint& h);
	std::vector<std::string> tilePaths;

	uint widthX = 0;
	uint heightY = 0;
	uint size = 0;
	uint* tileArray = nullptr;
	glm::vec3 dir = {0.0f, 0.0f, 15.0f};
	glm::vec3 chunkOffset;
};
