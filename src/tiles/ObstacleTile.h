#pragma once
#include "Tile.h"
#include "physics/ObstacleTrigger.h"

class ObstacleTile : public Tile
{
public:
	~ObstacleTile() override;
	void Init(const char* path, glm::vec3 pos, size_t index) override;
	void ResetPosition();
	glm::vec3 GetPosition() const;
	void UpdatePhysicsPosition(glm::vec3 chunkPos);
	ObstacleTrigger& GetCallback();
	void SetCallback(btGhostObject* ghostObject);

	void SetInitialPosition(glm::vec3 pos);

private:
	void AddATriggerBox();


	btGhostObject* ghostObject = nullptr;
	ObstacleTrigger* obstacleCallback = nullptr;
};
