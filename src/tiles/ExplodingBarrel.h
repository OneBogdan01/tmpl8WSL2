#pragma once
#include <array>

#include "Timer.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "model_loading/StaticMesh.h"
#include "model_loading/StaticModel.h"

class ExplodingBarrel
{
public:
	ExplodingBarrel();
	void AddParts();
	void RemoveParts();
	void Init(glm::vec3& pos);
	~ExplodingBarrel() = default;
	void AddADynamicObject(StaticMesh& mesh);
	void Update(float deltaTime);
	void Draw(Shader& shader);
	bool active = false;

private:
	std::array<btRigidBody*, 4> parts;
	int index = 0;
	 std::array<StaticModel*, 4> meshes={};
	glm::vec3 initialPosition;
	glm::vec3 position=glm::vec3(0,5,-5);
	Timer t;
};
