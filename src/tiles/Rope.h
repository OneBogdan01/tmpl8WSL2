#pragma once
#include "Line.h"
#include "template.h"
#include "Timer.h"
#include "glm/vec2.hpp"

//from my previos project
class Rope
{
public:
	Rope();
	~Rope();

	void Render();

	glm::vec3 GetMovingPartAtTime(glm::vec3 startPoint, float timeElapsed, const float leng);

	glm::vec3 GetMovingPart() const;

	glm::vec3* pGetMovingPart();

	void Update(float deltaTime);

	void Init(glm::vec3 _fixedPoint);

private:
	glm::vec3 points[4] = {};
	const float len[3] = {10.0f, 7.0f, 2.5f};
	const float lenMultiplier = 0.8f;
	const float multipler[3] = {0.1f, .6f, 1.0f};
	const uint resolution = 50;
	float totalLen = 0;
	const float frq = 1.8f;
	const float amp = 100.0f;
	const int halfWidth = 3;
	Timer* t = nullptr;
	Line line;
	float timeOffset = 0;
};
