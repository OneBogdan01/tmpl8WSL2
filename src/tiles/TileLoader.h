#pragma once
#include "ChunkOfTiles.h"


constexpr float TILE_SIZE = 5.0f;

//inspired by my old class from block A https://github.com/Tycro-Games/BlockA-Pitfall/blob/master/Scripts/Map/Tilemap.cpp
class TileLoader
{
public:
	~TileLoader();

	void Init(const char* csvPath);
	void DrawChunk(size_t index);

private:
	std::vector<ChunkOfTiles*> chunksOfTiles;

	void LoadCSVFile(const char* csvPath);

	bool hasEnding(const std::string& fullString, const std::string& ending);

	void ConvertCharToInt(const char* pch, uint& numberForm);
	void ExtractWidthHeight(const char* csvRaw, uint& w, uint& h);
	std::vector<std::string> tilePaths;
	uint widthX = 0;
	uint heightY = 0;
	uint size = 0;
	uint* tileArray = nullptr;
};
