#include "TileLoader.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "template.h"

namespace fs = std::filesystem;

TileLoader::~TileLoader()
{
	delete[] tileArray;
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
	std::string path = "assets/tiled/castle";

	for (const auto& entry : fs::directory_iterator(path))
	{
		string pathString = entry.path().string();
		if (hasEnding(pathString, ".ob"))
		{
			tilePaths.push_back(pathString);
		}
	}
	//alphabetically sort the tile paths
	sort(tilePaths.begin(), tilePaths.end());
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

	ExtractWidthHeight(tilemapRaw, widthX, heightY);
	//make a new array of unsigned intergers
	tileArray = new uint[widthX * heightY];
	size = widthX * heightY;
	//get to the start of the csv
	/*char* startOfCsv;
	for (int i = 0; i < strlen(tilemapRaw); i++)
	{
		if (tilemapRaw[i] == ',')
		{
			startOfCsv = tilemapRaw + i;
			break;
		}
	}*/
	char* startOfCsv = strstr(tilemapRaw, "csv") + 7;
	//get to the first element of the csv
	const char* pch = strtok(startOfCsv, ",");

	int index = 0;
	while (index < size) //stops when it reaches the end of the csv
	{
		uint numberForm = 0;
		ConvertCharToInt(pch, numberForm);
		cout << numberForm << '\n';
		tileArray[index++] = numberForm;
		pch = strtok(nullptr, ",\r\n");
	}

	delete[] tilemapRaw;
}
