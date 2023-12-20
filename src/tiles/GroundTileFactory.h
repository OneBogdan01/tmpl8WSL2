#pragma once

#include "TileFactory.h"

class GroundTileFactory : public TileFactory
{
public:
	//singleton
	static GroundTileFactory& GetInstance();

private:
	GroundTileFactory() = default;
};
