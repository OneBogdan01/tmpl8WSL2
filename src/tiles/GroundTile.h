#pragma once

#include "Tile.h"
#include "physics/GroundCollision.h"

class GroundTile : public Tile
{
public:
	GroundTile();
	~GroundTile() override = default ;
	//void ResetPosition(glm::vec3 pos);
	void Init(const char* path, glm::vec3 pos) override;
	void AddStaticRigidbody();
	glm::vec3 GetPosition() const;
	void UpdatePhysicsPosition(glm::vec3 chunkPos);

private:
	btRigidBody* rigidBody = nullptr;

	GroundCollision groundCallback = GroundCollision(GameObject::Ground);
};
