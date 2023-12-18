#include "TileLoader.h"
#ifdef _WINDOWS

#include <cstring>
#endif

#include <iostream>


#include <filesystem>
#include <random>

#include "Texture.h"


namespace fs = std::filesystem;

TileLoader::~TileLoader()
{
	for (auto& chunk : chunks)
	{
		delete chunk;
	}
}


//from https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
bool TileLoader::hasEnding(const std::string& fullString, const std::string& ending)
{
	if (fullString.length() >= ending.length())
	{
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}

	return false;
}

void TileLoader::Init()
{
	// Create a random device
	std::random_device rd;

	// Initialize a random number generator with the random device
	std::mt19937 g(rd());
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
#ifdef _WINDOWS

#endif
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


		for (int i = 0; i < heightY; i++)
		{
			for (int j = 0; j < widthX; j++)
			{
				uint index = j + i * widthX;
				uint modelIndex = tileArray[index];
				if (modelIndex) //index !=0
				{
					//so the index can start from 0
					modelIndex--;

					const glm::vec3 position = glm::vec3(static_cast<float>(j) * TILE_SIZE, 0.0f,
					                                     static_cast<float>(i) * TILE_SIZE)
						- offset;
					std::cout << position.x << " " << position.y << " " << position.z << "\n";
					chunk->LoadTile(index, tilePaths[modelIndex].c_str(), position);
				}
			}
		}

		if (k == 0)
			firstChunk = chunk;
		delete[]tileArray;
		chunks[k] = chunk;
	}
	//randomize order

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
		glm::vec3 chunkOff = glm::vec3(chunkOffset.x * k, chunkOffset.y * k, chunkOffset.z * k);

		if (k < numberOfActiveChunks)
			chunk->SetPosition(chunkOff);
		else
		{
			chunk->SetPosition(glm::vec3(static_cast<float>(-2)) * chunkOffset);
			chunk->Update(0);
		}
	}
}

void TileLoader::DrawChunks()
{
	for (size_t i = 0; i < numberOfActiveChunks; i++)
	{
		Chunk* chunk = chunks[i];
		chunk->Draw();
	}
}

void TileLoader::Update(float deltaTime)
{
	glm::vec3 newOffset = glm::vec3(0.0f);
	for (size_t i = 0; i < numberOfActiveChunks; i++)
	{
		newOffset.z = dir.z * speed * deltaTime;

		Chunk* chunk = chunks[i];
		//this chunk needs to be disabled
		if (chunk->GetPosition().z > 2 * TILE_SIZE * heightY)
		{
			//2 is the amount of tiles behind the player
			newOffset.z = -TILE_SIZE * heightY * (numberOfActiveChunks - 2);

			/*chunk->Translate(newOffset);
			chunk->Update(deltaTime);*/

			int randomIndex = rand() % chunks.size();
			if (randomIndex < numberOfActiveChunks)
				randomIndex = numberOfActiveChunks;
			swap(chunks[i], chunks[randomIndex]);
			chunk = chunks[i];
			//their original position
			chunk->ResetTiles();
		}
		chunk->Translate(newOffset);
		chunk->Update(deltaTime);
	}
}

void TileLoader::ConvertCharToInt(const char* pch, uint& numberForm)
{
	uint moveToRight = 1;
	for (size_t i = 0; i < strlen(pch); i++)
	{
		numberForm = numberForm * moveToRight + pch[i] - '0'; //conversion from char to int
		moveToRight = 10;
	}
}

void TileLoader::ExtractWidthHeight(const char* csvRaw, uint& w, uint& h)
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


void TileLoader::LoadCSVFile(const char* csvPath)
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
