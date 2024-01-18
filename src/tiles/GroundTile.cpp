#include "GroundTile.h"

#include "Camera.h"
#include "game.h"
#include "ModelTileFactory.h"

GroundTile::GroundTile() :
	Tile()
{
}


void GroundTile::Init(const char* path, const glm::vec3 pos, size_t index)
{
	index;
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



