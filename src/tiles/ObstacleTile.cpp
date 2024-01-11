#include "ObstacleTile.h"

#include "ChunkManager.h"
#include "game.h"
#include "ModelTileFactory.h"
#include "physics/World.h"


ObstacleTile::ObstacleTile():
Tile()
{
}

ObstacleTile::~ObstacleTile()
{
}

void ObstacleTile::ResetPosition()
{
	btTransform newTransform = ghostObject->getWorldTransform();

	newTransform.setOrigin(GetTileInitPosition());
	ghostObject->setWorldTransform(newTransform);
	/*for (auto& d : dynamicObjects)
		d->setWorldTransform(newTransform);*/
}

glm::vec3 ObstacleTile::GetPosition() const
{
	btVector3 position = ghostObject->getWorldTransform().getOrigin();
	position.setY(position.getY() - drawOffset.y);
	return BtVector3ToGlm(position);
}

void ObstacleTile::UpdatePhysicsPosition(const glm::vec3 chunkPos)
{
	btTransform newTransform = ghostObject->getWorldTransform();

	newTransform.setOrigin(btVector3(GlmToBtVector3(chunkPos + offset) + GetTileInitPosition()));
	ghostObject->setWorldTransform(newTransform);
	
}

ObstacleTrigger& ObstacleTile::GetCallback()
{
	return *obstacleCallback;
}

//TODO repeating code
void ObstacleTile::AddATriggerBox()
{
	//make a collision shape
	// Create a collision shape (e.g., a box shape for a rectangular tile)
	ModelTileFactory& factory = ModelTileFactory::GetInstance();
	btBoxShape* tileShape = World::CreateBoundingBoxModel(factory.GetModel(modelId)->GetMeshes(), TILE_SIZE * 2);

	// Create a motion state
	btTransform tileTransform;
	tileTransform.setIdentity();
	drawOffset = glm::vec3(0.0f, tileShape->getHalfExtentsWithMargin().y(), 0.0f);
	tileTransform.setOrigin(btVector3(initialPosition.x, initialPosition.y + drawOffset.y,
		initialPosition.z));


	ghostObject = new btGhostObject();
	ghostObject->setWorldTransform(tileTransform);
	ghostObject->setCollisionShape(tileShape);
	ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	ghostObject->setUserPointer(obstacleCallback);

	Game::world.AddTrigger(ghostObject);
}

void ObstacleTile::SetInitialPosition(const glm::vec3 pos)
{
	initialPosition = pos;
}

void ObstacleTile::SetCallback(btGhostObject* ghostObject)
{
	ghostObject->setUserPointer(obstacleCallback);
}

void ObstacleTile::Init(const char* path, const glm::vec3 pos, const size_t index)
{
	SetInitialPosition(pos);
	modelId = path;
	ModelTileFactory::GetInstance().CreateTileModel(path, pos);
	obstacleCallback = new ObstacleTrigger(GameObject::Obstacle, index);

	AddATriggerBox();

	////also add on trigger dynamic collisions for the parts
	//for (auto& mesh : ModelTileFactory::GetInstance().GetModel(path)->GetMeshes()) {

	//	AddADynamicObject(mesh);

	//}
}
void ObstacleTile::AddADynamicObject(StaticMesh& mesh)
{
	ModelTileFactory& factory = ModelTileFactory::GetInstance();
	// Create a collision shape for the dynamic object (e.g., a box shape)
	btConvexHullShape* dynamicShape = World::CreateBoundingConvexModel(mesh, TILE_SIZE * 2);


	// Create a motion state for the dynamic object
	btTransform dynamicTransform;
	dynamicTransform.setIdentity();
	dynamicTransform.setOrigin(btVector3(initialPosition.x, initialPosition.y, initialPosition.z));

	// Create a rigid body for the dynamic object
	btScalar mass = 1.0f;
	btVector3 dynamicInertia(0.0f, 0.0f, 0.0f);
	dynamicShape->calculateLocalInertia(mass, dynamicInertia);
	dynamicObjects[index] = new btRigidBody(mass, nullptr, dynamicShape, dynamicInertia);
	dynamicObjects[index]->setWorldTransform(dynamicTransform);
	btVector3 gravity = btVector3(0, 0, 0);
	dynamicObjects[index]->setGravity(gravity);
	// Add the rigid body to the Bullet dynamics world
	Game::world.AddRigidBody(dynamicObjects[index]);
	index++;
}