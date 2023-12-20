#pragma once
#include <iostream>
#include <unordered_map>

#include "Tile.h"

class TileFactory
{
public:
	virtual void CreateTileModel(const char* path, glm::vec3 initialPosition);

	StaticModel* GetModel(const char* path);

	void Draw(const char* path, Shader& shader) const;

private:
	StaticModel* LoadModel(const char* path, glm::vec3 initialPosition);

	std::unordered_map<const char*, StaticModel*> models = {};
};
