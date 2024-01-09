#pragma once
#include <string>

class RandomNames
{
public:
	enum Pattern
	{
		PreMidSuf,
		PreSuf,
		MidSuf,
		PreMid
	};

	static std::string GenerateRandomName();

private:
	static std::string GetRandomName(Pattern pattern);

	static std::string RandomName(std::string elements[], size_t size);

	//generate with chatGPT
	// Prefixes
	inline static std::string prefixes[14] = {
		"Jum",
		"Vin",
		"Mys",
		"Shad",
		"Ruin",
		"Ston",
		"Sav",
		"Tem",
		"Mos",
		"Ech",
		"Wil",
		"Lush",
		"Serp",
		"For"
	};

	// Middles
	inline static std::string middles[12] = {
		"cas",
		"ver",
		"ald",
		"over",
		"moss",
		"chase",
		"tem",
		"lush",
		"serp",
		"dan",
		"anc",
		"gott"
	};

	// Suffixes
	inline static std::string suffixes[8] = {
		"den",
		"ter",
		"per",
		"ed",
		"cer",
		"tra",
		"ent",
		"strider"
	};
};
