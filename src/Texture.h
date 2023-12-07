#pragma once

#include "template.h"

//modified from https://github.com/emeiri/ogldev/blob/master/Common/ogldev_texture.cpp
class Texture
{
public:
	Texture(Surface* texture);
	Texture(GLenum TextureTarget, const std::string& FileName);
	Texture();

	~Texture();
	void Init(const char* filePath);
	void Bind();
	void Bind(GLenum textureUnit);
	void Unbind();
	std::string type;
	std::string path;
	uint ID = 0;
	bool IsInit();

private:
	bool loaded = false;
	GLenum textureTarget;
	Surface texture;
};
