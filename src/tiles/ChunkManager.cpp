#include "ChunkManager.h"
#ifdef _WINDOWS

#include <cstring>
#endif

#include <iostream>


#include <filesystem>
#include <random>

#include "Texture.h"
#include "utilities/RandomNumberGenerator.h"

namespace fs = std::filesystem;

ChunkManager::~ChunkManager()
{
	for (auto& chunk : chunks)
	{
		delete chunk;
	}
}


//from https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
bool ChunkManager::hasEnding(const std::string& fullString, const std::string& ending)
{
	if (fullString.length() >= ending.length())
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}

	return false;
}

void ChunkManager::Init()
{
	srand(time(nullptr));


#ifdef _WINDOWS
	std::string path = "assets\\tiled\\castle";

#else

	std::string path = "assets/tiled/castle";
#endif


	for (const auto& entry : fs::directory_iterator(path))
	{
		std::string pathString = entry.path().string();
		if (hasEnding(pathString, ".ob"))
		{
			for (int x = 0; x < strlen(pathString.c_str()); x++)
				pathString[x] = putchar(tolower(pathString[x]));
			tilePaths.push_back(pathString);
		}
	}
	//alphabetically sort the tile paths
	sort(tilePaths.begin(), tilePaths.end());
	for (int i = 0; i < tilePaths.size(); i++)
	{
		cout << i << tilePaths[i] << '\n';
	}

	vector<string> tileMapPaths;
	for (const auto& entry : fs::directory_iterator("assets/tiled/"))
	{
		std::string pathString = entry.path().string();
		if (hasEnding(pathString, ".tmx"))
		{
			tileMapPaths.push_back(pathString);
		}
	}
	//alphabetically sort the tile paths
	sort(tileMapPaths.begin(), tileMapPaths.end());
	Chunk* firstChunk;
	for (int k = 0; k < NUMBER_OF_CHUNKS; k++)
	{
		LoadCSVFile(tileMapPaths[k].c_str());
		chunkOffset = glm::vec3(0.0f, 0.0f, -static_cast<float>(heightY) * TILE_SIZE);

		glm::vec3 offset = glm::vec3(static_cast<float>(widthX - 1) * TILE_SIZE / 2.0f, TILE_SIZE,
			static_cast<float>(heightY - 1) * TILE_SIZE);
		Chunk* chunk = new Chunk();

		//load Tiles
		for (int i = 0; i < heightY; i++)
		{
			for (int j = 0; j < widthX; j++)
			{
				uint index = j + i * widthX;
				uint modelIndex = tileArray[index];

				glm::vec3 position = glm::vec3(static_cast<float>(j) * TILE_SIZE, 0.0f,
					static_cast<float>(i) * TILE_SIZE)
					- offset;
				glm::vec3 coinPos = glm::vec3(0);
				if (modelIndex) //index !=0
				{
					//so the index can start from 0
					modelIndex--;


					std::cout << position.x << " " << position.y << " " << position.z << "\n";
					chunk->LoadTile(index, tilePaths[modelIndex].c_str(), position);


					coinPos.y += TILE_SIZE;
				}
				else
				{
					coinPos.y += TILE_SIZE * 2;
				}

			}
		}
		chunk->LoadProps(tilePaths[COINS_INDEX].c_str(), tilePaths[OBSTACLE_INDEX].c_str());
		if (k == 0)
		{
			firstChunk = chunk;
			chunk->HideChunk();
		}
		else
		{
			chunk->RandomizeChunk();
		}
		delete[]tileArray;
		chunks[k] = chunk;
	}

	//randomize order
	// Create a random device
	std::random_device rd;

	// Initialize a random number generator with the random device
	std::mt19937 g(rd());
	shuffle(chunks.begin(), chunks.end(), g);
	for (int i = 0; i < NUMBER_OF_CHUNKS; i++)
	{
		if (chunks[i] == firstChunk)
		{
			swap(chunks[0], chunks[i]);
			break;
		}
	}
	for (int k = 0; k < NUMBER_OF_CHUNKS; k++)
	{
		Chunk* chunk = chunks[k];

		if (k < NUMBER_OF_ACTIVE_CHUNKS) {
			glm::vec3 chunkOff = glm::vec3(chunkOffset.x * k, chunkOffset.y * k, chunkOffset.z * k);
			chunk->SetPosition(chunkOff-glm::vec3(0,0,3.0f));


		}
		else
		{
			chunk->SetPosition(glm::vec3(OFFSET_TILE_MULTIPLIER) * chunkOffset);
			chunk->Update(0);
		}
	}
}

void ChunkManager::DrawChunks()
{
	for (size_t i = 0; i < NUMBER_OF_ACTIVE_CHUNKS; i++)
	{
		Chunk* chunk = chunks[i];
		chunk->Draw();
	}
}

void ChunkManager::Update(const float deltaTime)
{
	glm::vec3 newOffset = glm::vec3(0.0f);
	for (size_t i = 0; i < NUMBER_OF_ACTIVE_CHUNKS; i++)
	{
		newOffset.z = dir.z * deltaTime;

		Chunk* chunk = chunks[i];


		//this chunk needs to be disabled graphics wise
		if (chunk->GetPosition().z > 1.1f * TILE_SIZE * heightY)
		{
		
			newOffset.z = -TILE_SIZE * heightY * (NUMBER_OF_ACTIVE_CHUNKS - 1);


			int randomIndex = static_cast<int>(RandomNumberGenerator::RandomFloat() * chunks.size());
			if (randomIndex < NUMBER_OF_ACTIVE_CHUNKS)
				randomIndex = NUMBER_OF_ACTIVE_CHUNKS;
			chunk->ResetRopes();
			swap(chunks[i], chunks[randomIndex]);
			chunk = chunks[i];
			//their original position
			chunk->ResetTiles();
			chunk->RandomizeChunk();
			if (endless)
				dir.z += increaseSpeed;
		}

		chunk->Translate(newOffset);
		//this chunk needs to be enabled physics wise
		if (chunk->GetPosition().z > -3.0f)
		{
			chunk->UpdateRB();

		}

		chunk->Update(deltaTime);
	}
}

void ChunkManager::SetDirectionZ(float _dir)
{
	dir.z = _dir;
}

void ChunkManager::SetEndless(float _dir)
{
	SetDirectionZ(_dir);
	endless = true;

}

void ChunkManager::ConvertCharToInt(const char* pch, uint& numberForm)
{
	uint moveToRight = 1;
	for (size_t i = 0; i < strlen(pch); i++)
	{
		numberForm = numberForm * moveToRight + pch[i] - '0'; //conversion from char to int
		moveToRight = 10;
	}
}

void ChunkManager::ExtractWidthHeight(const char* csvRaw, uint& w, uint& h)
{
	//widthX and heightY get
	char* tileRaw = new char[strlen(csvRaw) + 1];
	strcpy(tileRaw, csvRaw);

	char* getX = strstr(tileRaw, "width=") + strlen("width=");
	char* getY = strstr(tileRaw, "height=") + strlen("height=");
	const char* pX = strtok(getX, "\"");
	const char* pY = strtok(getY, "\"");

	ConvertCharToInt(pX, w);

	ConvertCharToInt(pY, h);
	delete[] tileRaw;
}


void ChunkManager::LoadCSVFile(const char* csvPath)
{
	//copy into a c style string
	char* tilemapRaw = new char[strlen(TextFileRead(csvPath).c_str()) + 1];
	strcpy(tilemapRaw, TextFileRead(csvPath).c_str());

	//print the whole file
	//cout << tilemapRaw << "\n";
	widthX = 0;
	heightY = 0;
	ExtractWidthHeight(tilemapRaw, widthX, heightY);
	//make a new array of unsigned intergers
	tileArray = new uint[widthX * heightY];
	size = widthX * heightY;
	//get to the start of the csv
#ifdef _WINDOWS

	char* startOfCsv = strstr(tilemapRaw, "csv") + 6;
#else
	char* startOfCsv = strstr(tilemapRaw, "csv") + 7;

#endif

	//get to the first element of the csv
	const char* pch = strtok(startOfCsv, ",");

	uint index = 0;
	while (index < size) //stops when it reaches the end of the csv
	{
		uint numberForm = 0;
		ConvertCharToInt(pch, numberForm);
		if (numberForm > 0)
			cout << numberForm - 1 << " which is the model: " << tilePaths[numberForm - 1] << '\n';
		tileArray[index++] = numberForm;
		pch = strtok(nullptr, ",\r\n");
	}

	delete[] tilemapRaw;
}
