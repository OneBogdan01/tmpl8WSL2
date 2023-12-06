#pragma once
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "model_loading/Model.h"

class Tile
{
public:
	void Init(const char* path, glm::vec3 pos);
	void AddStaticRigidbody();
	void LoadModel(const char* path);
	void Draw(Shader& shader) const;
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition() const;

private:
	Model* model = nullptr;
	glm::vec3 position = glm::vec3(0);
	btRigidBody* rigidBody = nullptr;
};
