#pragma once
#include "BulletWrapper.h"

#include "model_loading/StaticModel.h"
#include "physics/GameObject.h"

class Tile
{
public:
	virtual ~Tile() = default;
	void virtual Init(const char* path, glm::vec3 pos) = 0;
	btVector3 GetTileInitPosition();

	const char* GetId();

protected:
	const char* modelId = nullptr;

	glm::vec3 initialPosition = glm::vec3(0);
	glm::vec3 offset = glm::vec3(0.0f);
	glm::vec3 drawOffset = glm::vec3(0.0f);
};
