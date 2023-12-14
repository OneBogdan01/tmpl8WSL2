#include "Tile.h"


#include "game.h"
#include "TileLoader.h"
#include "physics/World.h"

void Tile::Init(const char* path, glm::vec3 pos)
{
	initialPosition = pos;

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
	drawOffset = glm::vec3(0.0f, tileShape->getHalfExtentsWithMargin().y(), 0.0f);
	tileTransform.setOrigin(btVector3(initialPosition.x, initialPosition.y + drawOffset.y,
	                                  initialPosition.z));
	btDefaultMotionState* tileMotionState = new btDefaultMotionState(tileTransform);

	// Create a rigid body
	btScalar mass =0.0; // Mass 0 for static objects
	btVector3 localInertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, tileMotionState, tileShape, localInertia);
	
	rigidBody = new btRigidBody(rbInfo);
	//set it as kinamatic
	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags()|btCollisionObject::CF_KINEMATIC_OBJECT);
	rigidBody->setActivationState(DISABLE_DEACTIVATION);



	// Add the rigid body to the dynamics world
	rigidBody->setUserPointer(&gameObject);

	Game::world.AddRigidBody(rigidBody);
}

void Tile::UpdatePhysicsPosition()
{
	btTransform newTransform;
	rigidBody->getMotionState()->getWorldTransform(newTransform);

	newTransform.setOrigin(newTransform.getOrigin() + btVector3(0, 0, offset.z));
	rigidBody->getMotionState()->setWorldTransform(newTransform);
	offset = glm::vec3(0.0f);
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

void Tile::SetOffset(glm::vec3 pos)
{
	offset = pos;
	if (offset != glm::vec3(0.0f))
		UpdatePhysicsPosition();
}

glm::vec3 Tile::GetPosition() const
{
	return BtVector3ToGlm(rigidBody->getWorldTransform().getOrigin()) - drawOffset;
}
