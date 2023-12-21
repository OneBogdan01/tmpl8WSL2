#pragma once
#include "Tile.h"
#include "physics/CoinTrigger.h"

class CoinTile : public Tile
{
public:
	~CoinTile() override;
	void Init(const char* path, glm::vec3 pos, size_t index) override;
	void ResetPosition();
	glm::vec3 GetPosition() const;
	void UpdatePhysicsPosition(glm::vec3 chunkPos);
	CoinTrigger& GetCallback();

private:
	void AddATriggerBox();

	btGhostObject* ghostObject = nullptr;
	CoinTrigger* coinCallBack = nullptr;
};
