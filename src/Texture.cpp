#include "Texture.h"

Texture::Texture(Surface* texture)
{
	this->texture = texture;
}

Texture::Texture()
{
}

Texture::~Texture()
{
	delete texture;
}

void Texture::Init(const char* filePath, bool alpha)
{
	texture->LoadRawData(filePath);
	//generate and bound
	glGenTextures(1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);

	// set the texture wrapping/filtering options (on the currently bound texture object) from https://learnopengl.com/Getting-started/Textures


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//make texture
	if (!alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		             texture->width, texture->height,
		             0, GL_RGB, GL_UNSIGNED_BYTE,
		             texture->rawPixels);
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		             texture->width, texture->height,
		             0, GL_RGBA, GL_UNSIGNED_BYTE,
		             texture->rawPixels);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	texture->FreeRawData();

	//unbound and free memory
	delete texture;
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
