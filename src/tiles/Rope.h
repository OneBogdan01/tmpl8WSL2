#pragma once
#include "Tile.h"

#include "Line.h"
#include "Timer.h"
#include "glm/fwd.hpp"

#include "glm/vec2.hpp"
#include "physics/RopeTrigger.h"

//from my previos project
class Rope : public Tile
{
public:

	void Render(glm::vec3 position);
	void UpdatePhysicsPosition(glm::vec3 chunkPos);
	void ResetPosition();

	RopeTrigger& GetCallback();
	void SetCallback(btGhostObject* ghostObject);
	Rope();
	~Rope() override;

	glm::vec3 DrawRope(const glm::vec3 a, float offsetX, btVector3 color);


	void Init(const char* path, glm::vec3 pos, size_t index) override;
	void AddATriggerBox();

	glm::vec3 GetMovingPartAtTime(glm::vec3 startPoint, float timeElapsed, const float leng);

	glm::vec3 GetMovingPart() const;

	glm::vec3* pGetMovingPart();
	void ActivateMovement();
	void SimulateRope(float deltaTime);

	void Update(float deltaTime);

	void ChangePosition(const glm::vec3& pos);
	void DeactivateMovement();


	bool shouldMove = false;
	bool deactivate = false;
private:
	size_t index;
	RopeTrigger* callback=nullptr;
	btGhostObject* ghostObject = nullptr;

	glm::vec3 points[4] = {};
	const float len[3] = { 10.0f, 7.0f, 2.5f };
	const float lenMultiplier = 0.8f;
	const float multipler[3] = { 0.1f, .6f, 0.70f };
	const uint resolution = 32;
	float totalLen = 0;
	const float frq = 1.8f;
	const float amp = 150.0f;
	const int halfWidth = 3;
	Timer* t = nullptr;
	Timer* timeToDisable = nullptr;
	bool startDisableing = false;
	Line line;
	float timeOffset = 0;
	const float offset = 2.0f;
	
};
