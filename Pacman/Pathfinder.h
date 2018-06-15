#pragma once
#include "PathmapTile.h"
#include <list>
#include "BigDot.h"

class Pathfinder
{
public:
	static bool Pathfind(World* world, PathmapTile* startTile, PathmapTile* endTile, std::list<PathmapTile*>& out);
};

