#pragma once

#include "Tile.h"
#include "physics/GroundCollision.h"

class GroundTile : public Tile
{
public:
	GroundTile();
	~GroundTile() override = default ;
	//void ResetPosition(glm::vec3 pos);
	void Init(const char* path, glm::vec3 pos, size_t index) override;
	void SetUserPointer(btRigidBody* body);


private:

	GroundCollision groundCallback = GroundCollision(GameObject::Ground);
};
