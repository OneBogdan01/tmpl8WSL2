#pragma once

#include "Tile.h"

class GroundTile : public Tile
{
public:
	GroundTile();
	~GroundTile() override = default ;
	//void ResetPosition(glm::vec3 pos);
	void Init(const char* path, glm::vec3 pos) override;
	void AddStaticRigidbody();
	void ResetPosition();
	glm::vec3 GetPosition() const;
	void UpdatePhysicsPosition(glm::vec3 chunkPos);

private:
	GameObject gameObject = GameObject(GameObject::Ground);
};
