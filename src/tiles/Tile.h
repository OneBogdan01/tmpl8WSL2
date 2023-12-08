#pragma once
#include <BulletDynamics/Dynamics/btRigidBody.h>

#include "model_loading/StaticModel.h"
#include "physics/GameObject.h"

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
	GameObject gameObject = GameObject(GameObject::Box);
	StaticModel* model = nullptr;
	glm::vec3 position = glm::vec3(0);
	btRigidBody* rigidBody = nullptr;
};
