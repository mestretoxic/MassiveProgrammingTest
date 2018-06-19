#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "PathmapTile.h"
#include <list>

class Ghost;
class PathmapTile;

class Pathfinder
{
public:
	static bool Pathfind(World* world, const Ghost* ghost, PathmapTile* startTile, PathmapTile* endTile, bool ignoreSpawn, std::list<PathmapTile*>& out);
private:
	static PathmapTile* GetValidTileAt(World* world, const Ghost* ghost, int x, int y, bool ignoreSpawn);
};

#endif