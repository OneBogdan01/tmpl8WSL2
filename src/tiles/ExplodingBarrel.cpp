#include "ExplodingBarrel.h"
#include "template.h"
#include "ChunkManager.h"
#include "game.h"
#include <string>
#include "model_loading/StaticMesh.h"
#include "physics/World.h"
#include "utilities/RandomNumberGenerator.h"

ExplodingBarrel::ExplodingBarrel() : parts(), initialPosition(), position()
{
	if (meshes[0] == nullptr)
	{
		meshes[0] = new StaticModel("assets/tiled/part1.ob");

		meshes[1] = new StaticModel(("assets/tiled/part2.ob"));
		meshes[2] = new StaticModel(("assets/tiled/part3.ob"));
		meshes[3] = new StaticModel(("assets/tiled/part4.ob"));
		for (int i = 0; i < 4; i++) {
			auto& model = meshes[i];
			for (auto& mesh : model->GetMeshes()) {
				mesh.BakeDirectionalLighting();
				AddADynamicObject(mesh);
			}
			model->SetUpMeshes();
		}

	}



}


void ExplodingBarrel::AddParts()
{
	for (auto& part : parts)
		Game::world.AddRigidBody(part, 1, 1);
}

void ExplodingBarrel::RemoveParts()
{
	for (auto& part : parts)
		Game::world.RemoveRigidBody(part);
}

void ExplodingBarrel::Init(glm::vec3& pos)
{
	//set to parts
	position = pos;
	for (size_t i = 0; i < parts.size(); i++)
	{


		// Reset the position
		btTransform transform = parts[i]->getWorldTransform();
		transform.setIdentity();
		//parts[i]->getMotionState()->setWorldTransform(transform);
		transform.setOrigin(GlmToBtVector3(pos));
		parts[i]->setWorldTransform(transform);
		parts[i]->clearForces();

	}
	AddParts();
	float sign = RandomNumberGenerator::RandomFloat() > 0.5f ? 1.0f : -1.0f;
	float y = RandomNumberGenerator::RandomFloat() + 0.3f;
	int i = 0;
	float x = 0;

	for (auto& part : parts) {


		if (i % 2 == 0)
			x = RandomNumberGenerator::RandomFloat() * 1;
		else
			x = RandomNumberGenerator::RandomFloat() * -1;
		i++;
		float z = RandomNumberGenerator::RandomFloat() * sign;

		part->setLinearVelocity(btVector3(0, 0, 0));
		part->setAngularVelocity(btVector3(0, 0, 0));
		//part->applyCentralForce(btVector3(0, -10, 0));

		float magnitude = 25.0f;

		btVector3 forceDirection = btVector3(x, y, z);



		// Apply force to the rigid body.
		part->applyCentralImpulse(forceDirection.normalized() * btScalar(magnitude));


	}

	t.reset();
	active = true;
}


void ExplodingBarrel::AddADynamicObject(StaticMesh& mesh)
{
	// Create a collision shape for the dynamic object (e.g., a box shape)
	btConvexHullShape* dynamicShape = World::CreateBoundingConvexModel(mesh, TILE_SIZE * 2);


	// Create a motion state for the dynamic object
	btTransform dynamicTransform;
	dynamicTransform.setIdentity();
	dynamicTransform.setOrigin(btVector3(0, 0, 0));
	// Create a rigid body for the dynamic object
	btScalar mass = 1.0f;
	btVector3 dynamicInertia(0.0f, 0.0f, 0.0f);
	dynamicShape->calculateLocalInertia(mass, dynamicInertia);
	// Use motion state to initialize the rigid body's initial transform
	btDefaultMotionState* motionState = new btDefaultMotionState(dynamicTransform);
	parts[index] = new btRigidBody(mass, motionState, dynamicShape, dynamicInertia);




	index++;

}

void ExplodingBarrel::Disable()
{
	RemoveParts();
	active = false;
}

void ExplodingBarrel::Update(float /*deltaTime*/)
{
	//deltaTime;
	if (active)
	{
		for (auto& part : parts) {
			part->setGravity(btVector3(0, -25, 0));
			part->applyGravity();
			//part->applyCentralForce(btVector3(0, -10, 0));
		}
		if (t.elapsed() > 1.7f)
		{
			Disable();
		}
	}
}
void ExplodingBarrel::Draw(Shader& shader)
{
	if (!active)
		return;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		glm::vec3 partPos = BtVector3ToGlm(parts[i]->getWorldTransform().getOrigin());

		btQuaternion rotationQuat = parts[i]->getWorldTransform().getRotation();
		glm::quat rotation = glm::quat(rotationQuat.w(), rotationQuat.x(), rotationQuat.y(), rotationQuat.z());

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, partPos);
		model = glm::rotate(model, glm::angle(rotation), glm::axis(rotation));
		model = glm::scale(model, glm::vec3(TILE_SIZE * 2));
		shader.SetMat4x4("model", model);
		meshes[i]->Draw(shader);
	}

}
