#pragma once
#include "Chunk.h"
#include "model_loading/ogldev_types.h"


constexpr float TILE_SIZE = 5.0f;

//inspired by my old class from block A https://github.com/Tycro-Games/BlockA-Pitfall/blob/master/Scripts/Map/Tilemap.cpp
class TileLoader
{
public:
	~TileLoader();

	void Init(const char* csvPath);
	void DrawChunk(size_t index);
	void Update(float deltaTime);

private:
	std::vector<Chunk*> chunksOfTiles;

	void LoadCSVFile(const char* csvPath);

	bool hasEnding(const std::string& fullString, const std::string& ending);

	void ConvertCharToInt(const char* pch, uint& numberForm);
	void ExtractWidthHeight(const char* csvRaw, uint& w, uint& h);
	std::vector<std::string> tilePaths;
	uint widthX = 0;
	uint heightY = 0;
	uint size = 0;
	uint* tileArray = nullptr;
	glm::vec3 dir = {0.0f, 0.0f, 1.0f};
	float speed = 10.0f;
	glm::vec3 chunkOffset;
};
