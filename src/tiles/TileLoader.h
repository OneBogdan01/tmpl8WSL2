#pragma once
#include <string>
#include <vector>

class TileLoader
{
public:
	~TileLoader();
	bool hasEnding(const std::string& fullString, const std::string& ending);
	void Init();
	void ConvertCharToInt(const char* pch, uint& numberForm);
	void ExtractWidthHeight(const char* csvRaw, uint& w, uint& h);
	void LoadCSVFile(const char* csvPath);

private:
	std::vector<std::string> tilePaths;
	uint* tileArray;
	uint widthX;
	uint heightY;
	uint size;
};
