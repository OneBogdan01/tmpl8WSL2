#include "CoinTile.h"

#include "Camera.h"

#include "ChunkManager.h"
#include "game.h"
#include "GroundTileFactory.h"
#include "physics/CoinTrigger.h"
#include "physics/World.h"

void CoinTile::Init(const char* path, glm::vec3 pos, size_t index)
{
	initialPosition = pos;
	modelId = path;
	GroundTileFactory::GetInstance().CreateTileModel(path, pos);
	coinCallBack = new CoinTrigger(GameObject::Coin, index);

	AddATriggerBox();
}

CoinTrigger& CoinTile::GetCallback()
{
	return *coinCallBack;
}

void CoinTile::AddATriggerBox()
{
	//make a collision shape
	// Create a collision shape (e.g., a box shape for a rectangular tile)
	GroundTileFactory& factory = GroundTileFactory::GetInstance();
	btBoxShape* tileShape = World::CreateBoundingBoxModel(factory.GetModel(modelId)->GetMeshes(), TILE_SIZE);

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

	ghostObject->setUserPointer(coinCallBack);

	Game::world.AddTrigger(ghostObject);
}

CoinTile::~CoinTile()
{
	delete coinCallBack;
}

void CoinTile::ResetPosition()
{
	btTransform newTransform = ghostObject->getWorldTransform();

	newTransform.setOrigin(GetTileInitPosition());
	ghostObject->setWorldTransform(newTransform);
}

glm::vec3 CoinTile::GetPosition() const
{
	btVector3 position = ghostObject->getWorldTransform().getOrigin();
	position.setY(position.getY() - drawOffset.y);
	return BtVector3ToGlm(position);
}


void CoinTile::UpdatePhysicsPosition(glm::vec3 chunkPos)
{
	btTransform newTransform = ghostObject->getWorldTransform();

	newTransform.setOrigin(btVector3(GlmToBtVector3(chunkPos + offset) + GetTileInitPosition()));
	ghostObject->setWorldTransform(newTransform);
}
