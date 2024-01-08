#include "Tile.h"


#include "Camera.h"
#include "game.h"


btVector3 Tile::GetTileInitPosition()
{
	return btVector3(initialPosition.x, initialPosition.y + drawOffset.y,
	                 initialPosition.z);
}

glm::vec3 Tile::GetPosition(const glm::vec3 chunkPos) const
{
	const glm::vec3 pos = chunkPos + initialPosition;

	return pos;
}


const char* Tile::GetId()
{
	return modelId;
}
