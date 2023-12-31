#include "Texture.h"



#include "model_loading/ogldev_util.h"

Texture::Texture(Surface* texture): textureTarget(0)
{
	this->texture = *texture;
}

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	textureTarget = TextureTarget;
	Init(FileName.c_str());
}

Texture::Texture()
= default;

Texture::~Texture()
{
}

void Texture::Init(const char* filePath)
{
	texture.LoadRawData(filePath);
	//generate and bound
	glGenTextures(1, &ID);

	glBindTexture(GL_TEXTURE_2D, ID);

	// set the texture wrapping/filtering options (on the currently bound texture object) from https://learnopengl.com/Getting-started/Textures


	switch (texture.numberChannels)
	{
	case 1:
		glTexImage2D(textureTarget, 0, GL_RED, texture.width, texture.height, 0, GL_RED, GL_UNSIGNED_BYTE,
		             texture.rawPixels);
		break;

	case 2:
		glTexImage2D(textureTarget, 0, GL_RG, texture.width, texture.height, 0, GL_RG, GL_UNSIGNED_BYTE,
		             texture.rawPixels);
		break;

	case 3:
		glTexImage2D(textureTarget, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		             texture.rawPixels);
		break;

	case 4:
		glTexImage2D(textureTarget, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		             texture.rawPixels);
		break;

	default:
		NOT_IMPLEMENTED;
	}
	glGenerateMipmap(textureTarget);

	glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	texture.FreeRawData();
	////unbound and free memory
	//delete texture;
	Unbind();
	loaded = true;
}


void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::IsInit()
{
	return loaded;
}
