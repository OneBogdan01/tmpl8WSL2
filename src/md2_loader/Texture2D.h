#pragma once


#include <string>
#include "template.h"
using std::string;

class Texture2D
{
public:
	Texture2D();
	virtual ~Texture2D();

	bool loadTexture(const char* fileName, bool generateMipMaps = true);
	void bind(GLuint texUnit = 0);
	void unbind();

private:
	Texture2D(const Texture2D& rhs) = default;
	Texture2D& operator =(const Texture2D& rhs) = default;

	uint mTexture;
};
