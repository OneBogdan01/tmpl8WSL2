#include "Tile.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>

#include "game.h"
#include "TileLoader.h"
#include "physics/World.h"

void Tile::Init(const char* path, glm::vec3 pos)
{
	position = pos;

	LoadModel(path);
	AddStaticRigidbody();
}

void Tile::AddStaticRigidbody()
{
	//make a collision shape
	// Create a collision shape (e.g., a box shape for a rectangular tile)
	btBoxShape* tileShape = World::CreateBoundingBoxModel(model->GetMeshes(), TILE_SIZE);

	// Create a motion state
	btTransform tileTransform;
	tileTransform.setIdentity();
	tileTransform.setOrigin(btVector3(position.x, position.y + tileShape->getHalfExtentsWithMargin().y(),
	                                  position.z));
	btDefaultMotionState* tileMotionState = new btDefaultMotionState(tileTransform);

	// Create a rigid body
	btScalar mass = 0.0; // Mass 0 for static objects
	btVector3 localInertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, tileMotionState, tileShape, localInertia);
	rigidBody = new btRigidBody(rbInfo);

	// Add the rigid body to the dynamics world
	Game::world.AddRigidBody(rigidBody);
}

void Tile::LoadModel(const char* path)
{
	model = new StaticModel(path);
}

void Tile::Draw(Shader& shader) const
{
	if (model != nullptr)
		model->Draw(shader);
}

void Tile::SetPosition(glm::vec3 pos)
{
	position = pos;
}

glm::vec3 Tile::GetPosition() const
{
	return position;
}
