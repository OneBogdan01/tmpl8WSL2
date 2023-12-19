#pragma once
#include "Chunk.h"
#include "model_loading/ogldev_types.h"


constexpr float TILE_SIZE = 5.0f;
constexpr size_t NUMBER_OF_CHUNKS = 5;

//inspired by my old class from block A https://github.com/Tycro-Games/BlockA-Pitfall/blob/master/Scripts/Map/Tilemap.cpp
class TileLoader
{
public:
	~TileLoader();

	void Init();
	void DrawChunks();
	void Update(float deltaTime);

private:
	std::array<Chunk*, NUMBER_OF_CHUNKS> chunks = {};
	//memory test
	//std::array<Chunk*, sizeof(Chunk> test = {};
	const uint numberOfActiveChunks = 3;


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
	float speed = 15.0f;
	glm::vec3 chunkOffset;
};
