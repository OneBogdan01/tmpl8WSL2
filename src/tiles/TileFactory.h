#pragma once
#include <unordered_map>
#include <vector>>
#include "Tile.h"

class TileFactory
{
public:
	TileFactory();
	virtual ~TileFactory() = default;
	virtual void CreateTileModel(const char* path, glm::vec3 initialPosition);

	StaticModel* GetModel(const char* path);

	void Draw(const char* path, Shader& shader) const;
	void DrawInstanced(const char* path, ::Shader& shader, std::vector<glm::mat4>& modelMatrices) const;
	//static TileFactory& GetInstance();

private:
	StaticModel* LoadModel(const char* path, glm::vec3 initialPosition);
	unsigned int instancedBuffer;

	std::unordered_map<const char*, StaticModel*> models = {};
};
