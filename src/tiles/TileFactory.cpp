#include "TileFactory.h"

#include <iostream>

void TileFactory::CreateTileModel(const char* path, glm::vec3 initialPosition)
{
	if (models.find(path) == models.end())
	{
		models[path] = LoadModel(path, initialPosition);
	}
}

StaticModel* TileFactory::GetModel(const char* path)
{
	auto model = models.find(path);
	if (model == models.end())
	{
		std::cout << "Model not found: " << path << std::endl;
	}

	return models[path];
}

void TileFactory::Draw(const char* path, Shader& shader) const
{
	models.at(path)->Draw(shader);
}


StaticModel* TileFactory::LoadModel(const char* path, glm::vec3 initialPosition)
{
	const auto model = new StaticModel(path);

	for (auto& mesh : model->GetMeshes())
	{
		mesh.BakeLighting(initialPosition);
	}

	model->SetUpMeshes();
	return model;
}
