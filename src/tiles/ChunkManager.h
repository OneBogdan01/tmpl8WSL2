#pragma once
#include <vector>

#include "Chunk.h"
#include "TerrainChunk.h"
//#pragma warning(push, 0)
#include "model_loading/ogldev_types.h"
//#pragma warning(pop)

constexpr float TILE_SIZE = 5.0f;
constexpr float OFFSET_TILE_MULTIPLIER = -1.1f;
constexpr size_t NUMBER_OF_CHUNKS = 9;
constexpr size_t NUMBER_OF_TERRAIN_CHUNKS = 3;
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
	void DrawTerrainChunks();
	void DrawChunks();
	void Update(float deltaTime);
	void SetDirectionZ(float _dir);
	void SetEndless(float _dir);
	void Reset();

	static uint GetHeight()
	{
		return heightY;
	}

	static uint GetWidth()
	{
		return widthX;
	}

	static inline float changeRope = 0;
	std::vector<std::string> modelPaths;
	void ParseModelPaths();

private:
	float maxZ = -100;
	Chunk* chunkWithMaxZ = nullptr;
	std::array<Chunk*, NUMBER_OF_CHUNKS> chunks = {};
	std::array<TerrainChunk*, NUMBER_OF_TERRAIN_CHUNKS> terrainChunks = {};
	Chunk* firstChunk;

	//helper functions for parsing
	void LoadCSVFile(const char* csvPath);
	bool hasEnding(const std::string& fullString, const std::string& ending);
	void ConvertCharToInt(const char* pch, uint& numberForm);
	void ExtractWidthHeight(const char* csvRaw, uint& w, uint& h);
	std::vector<std::string> tilePaths;
	bool endless = false;
	static inline uint widthX = 0;
	static inline uint heightY = 0;
	uint size = 0;
	uint* tileArray = nullptr;
	glm::vec3 dir = {0.0f, 0.0f, 15.0f};
	const float increaseSpeed = 0.1f;
	glm::vec3 chunkOffset = glm::vec3(0.0f);
	 Shader* terrainShader = nullptr;

#ifdef _WINDOWS
	std::string path = "assets\\tiled\\castle";

#else

	std::string path = "assets/tiled/castle";
#endif
};
