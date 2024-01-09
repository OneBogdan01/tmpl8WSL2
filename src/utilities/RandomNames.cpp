#include "RandomNames.h"
#include "RandomNumberGenerator.h"

std::string RandomNames::GenerateRandomName()
{
	return GetRandomName(static_cast<Pattern>(RandomNumberGenerator::RandomFloat() * static_cast<float>(PreMid)));
}

std::string RandomNames::GetRandomName(Pattern pattern)
{
	std::string name;
	size_t preSize = std::size(prefixes);
	size_t midSize = std::size(middles);
	size_t sufSize = std::size(suffixes);
	switch (pattern)
	{
	case PreMidSuf:
		name = RandomName(prefixes, preSize) + RandomName(middles, midSize) + RandomName(suffixes, sufSize);
		break;
	case PreMid:
		name = RandomName(prefixes, preSize) + RandomName(middles, midSize) + RandomName(middles, midSize);

		break;
	case PreSuf:
		name = RandomName(prefixes, preSize) + RandomName(suffixes, sufSize);
		break;
	case MidSuf:
		name = RandomName(middles, midSize) + RandomName(middles, midSize) + RandomName(suffixes, sufSize);
		break;
	}
	name[0] = static_cast<char>(std::toupper(name[0]));
	return name;
}

std::string RandomNames::RandomName(std::string elements[], const size_t size)
{
	std::string name;

	const int randomIndex = static_cast<int>(RandomNumberGenerator::RandomFloat() * static_cast<float>(size - 1));

	return elements[randomIndex];
}
