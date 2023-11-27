#pragma once

#include "template.h"

class Texture
{
public:
	Texture(Surface* texture);
	Texture();

	~Texture();
	void Init(const char* filePath, bool alpha = false);
	void Bind();
	void Unbind();
	std::string type;
	std::string path;
	uint ID = 0;

private:
	Surface* texture = nullptr;
};
