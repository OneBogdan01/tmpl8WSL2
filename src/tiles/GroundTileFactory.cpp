#include "GroundTileFactory.h"

#include <iostream>

#include "Camera.h"


GroundTileFactory& GroundTileFactory::GetInstance()
{
	static GroundTileFactory* instance = new GroundTileFactory();

	return *instance;
}
