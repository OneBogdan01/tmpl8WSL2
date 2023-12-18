#pragma once
#include "BulletWrapper.h"

#include "model_loading/StaticModel.h"
#include "physics/GameObject.h"

class Tile
{
public:
	void Init(const char* path, glm::vec3 pos);
	btVector3 GetTileOffset();
	void ResetPosition(glm::vec3 pos);
	void AddStaticRigidbody();
	void UpdatePhysicsPosition(glm::vec3 chunkPos);

	void LoadModel(const char* path);
	void Draw(Shader& shader) const;
	void Translate(glm::vec3 pos);
	glm::vec3 GetPosition() const;

private:
	GameObject gameObject = GameObject(GameObject::Box);
	StaticModel* model = nullptr;
	glm::vec3 initialPosition = glm::vec3(0);
	glm::vec3 offset = glm::vec3(0.0f);
	glm::vec3 drawOffset = glm::vec3(0.0f);
	btRigidBody* rigidBody = nullptr;
};
