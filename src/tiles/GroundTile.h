#pragma once

#include "Tile.h"
#include "physics/GroundCollision.h"

class GroundTile : public Tile
{
public:
	GroundTile();
	~GroundTile() override = default ;
	//void ResetPosition(glm::vec3 pos);
	auto Init(const char* path, glm::vec3 pos, size_t index) -> void override;
	void SetUserPointer(btRigidBody* body);
	void SetRigidBody(btRigidBody* body);
	void AddStaticRigidbody();
	glm::vec3 GetPosition(glm::vec3 chunkPos) const;
	void UpdatePhysicsPosition(glm::vec3 chunkPos);

private:
	btRigidBody* rigidBody = nullptr;

	GroundCollision groundCallback = GroundCollision(GameObject::Ground);
};
