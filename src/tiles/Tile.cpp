#include "Tile.h"

void Tile::LoadModel(const char* path)
{
	model = new Model(path);
}

void Tile::Draw(Shader& shader) const
{
	if (model != nullptr)
		model->Draw(shader);
}

void Tile::SetPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 Tile::GetPosition() const
{
	return position;
}
