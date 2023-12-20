#include "Tile.h"


#include "Camera.h"
#include "game.h"


btVector3 Tile::GetTileInitPosition()
{
	return btVector3(initialPosition.x, initialPosition.y + drawOffset.y,
	                 initialPosition.z);
}

const char* Tile::GetId()
{
	return modelId;
}
