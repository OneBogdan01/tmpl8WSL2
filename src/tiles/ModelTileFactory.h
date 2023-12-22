#pragma once

#include "TileFactory.h"

class ModelTileFactory : public TileFactory
{
public:
	//singleton
	static ModelTileFactory& GetInstance();

private:
	ModelTileFactory() = default;
};
