#include "GroundTileFactory.h"

#include <iostream>



GroundTileFactory& GroundTileFactory::GetInstance()
{
	static GroundTileFactory* instance = new GroundTileFactory();

	return *instance;
}
