#pragma once
#include "Tile.h"
#include "physics/CoinTrigger.h"

class CoinTile : public Tile
{
public:
	void Init(const char* path, glm::vec3 pos) override;
	void ResetPosition();
	glm::vec3 GetPosition() const;
	void UpdatePhysicsPosition(glm::vec3 chunkPos);

private:
	void AddATriggerBox();

	btGhostObject* ghostObject = nullptr;
	CoinTrigger coinCallBack = CoinTrigger(GameObject::Coin);
};
