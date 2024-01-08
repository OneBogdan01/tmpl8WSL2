#pragma once
#include "BulletWrapper.h"

#include "model_loading/StaticModel.h"
#include "physics/GameObject.h"

class Tile
{
public:
	virtual ~Tile() = default;
	Tile() = default;
	virtual void  Init(const char* path, glm::vec3 pos, size_t index) = 0;
	btVector3 GetTileInitPosition();
	glm::vec3 GetPosition(glm::vec3 chunkPos) const;


	const char* GetId();
	glm::vec3 initialPosition = glm::vec3(0);
	glm::vec3 offset = glm::vec3(0.0f);


protected:
	const char* modelId = nullptr;

	glm::vec3 drawOffset = glm::vec3(0.0f);
};
