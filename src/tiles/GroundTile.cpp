#include "GroundTile.h"

#include "Camera.h"
#include "game.h"
#include "ModelTileFactory.h"
#include "ChunkManager.h"

GroundTile::GroundTile() :
	Tile()
{
}


void GroundTile::Init(const char* path, const glm::vec3 pos, size_t index)
{
	initialPosition = pos;
	modelId = path;
	ModelTileFactory::GetInstance().CreateTileModel(path, pos);

	//LoadModel(path);
	//AddStaticRigidbody();
}
void GroundTile::SetUserPointer(btRigidBody* body)
{
	body->setUserPointer(&groundCallback);
}

void GroundTile::SetRigidBody(btRigidBody* body)
{
	rigidBody = body;
	SetUserPointer(rigidBody);
}

void GroundTile::AddStaticRigidbody()
{
	//make a collision shape
	// Create a collision shape (e.g., a box shape for a rectangular tile)
	ModelTileFactory& factory = ModelTileFactory::GetInstance();
	btBoxShape* tileShape = World::CreateBoundingBoxModel(factory.GetModel(modelId)->GetMeshes(), TILE_SIZE);

	// Create a motion state
	btTransform tileTransform;
	tileTransform.setIdentity();
	drawOffset = glm::vec3(0.0f, tileShape->getHalfExtentsWithMargin().y(), 0.0f);
	tileTransform.setOrigin(btVector3(initialPosition.x, initialPosition.y + drawOffset.y,
		initialPosition.z));
	btDefaultMotionState* tileMotionState = new btDefaultMotionState(tileTransform);

	// Create a rigid body
	btScalar mass = 0.0; // Mass 0 for static objects
	btVector3 localInertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, tileMotionState, tileShape, localInertia);

	rigidBody = new btRigidBody(rbInfo);
	//set it as kinamatic
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);


	// Add the rigid body to the dynamics world
	rigidBody->setUserPointer(&groundCallback);

	Game::world.AddRigidBody(rigidBody);
}


glm::vec3 GroundTile::GetPosition(const glm::vec3 chunkPos) const
{

	glm::vec3 pos = chunkPos + initialPosition;

	return pos;
}

void GroundTile::UpdatePhysicsPosition(const glm::vec3 chunkPos)
{
	btTransform newTransform;
	rigidBody->getMotionState()->getWorldTransform(newTransform);

	newTransform.setOrigin(btVector3(GlmToBtVector3(chunkPos) + GetTileInitPosition()));
	rigidBody->getMotionState()->setWorldTransform(newTransform);
}
