#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "PathmapTile.h"
#include <list>

class PathmapTile;

class Pathfinder
{
public:
	static bool Pathfind(World* world, PathmapTile* startTile, PathmapTile* endTile, bool ignoreSpawn, std::list<PathmapTile*>& out);
private:
	static PathmapTile* GetValidTileAt(World* world, int x, int y, bool ignoreSpawn);
};

#endif