#pragma once
#include "model_loading/Model.h"

class Tile
{
public:
	void LoadModel(const char* path);
	void Draw(Shader& shader) const;
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition() const;

private:
	Model* model = nullptr;
	glm::vec3 position = glm::vec3(0);
};
