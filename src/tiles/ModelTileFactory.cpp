#include "ModelTileFactory.h"

#include <iostream>



ModelTileFactory& ModelTileFactory::GetInstance()
{
	static ModelTileFactory* instance = new ModelTileFactory();

	return *instance;
}
