#include "Pathfinder.h"
#include "PathmapTile.h"
#include <list>
#include "World.h"

bool ListDoesNotContain(PathmapTile* tileToFind, std::list<PathmapTile*>& list)
{
	for (auto& tile : list)
	{
		if (tile == tileToFind)
			return false;
	}
	return true;
}

bool SortFromGhostSpawn(PathmapTile* a, PathmapTile* b)
{
	int la = abs(a->x - 13) + abs(a->y - 13);
	int lb = abs(b->x - 13) + abs(b->y - 13);

    return la < lb;
}

bool Pathfinder::Pathfind(World* world, PathmapTile* startTile, PathmapTile* endTile, std::list<PathmapTile*>& out)
{
	if (!startTile || !endTile)
		return false;

	startTile->isVisitedFlag = true;

	if (startTile->isBlockingFlag)
		return false;

	if (startTile == endTile)
		return true;

	std::list<PathmapTile*> neighborList;

	PathmapTile* up = world->GetTile(startTile->x, startTile->y - 1);
	if (up && !up->isVisitedFlag && !up->isBlockingFlag && ListDoesNotContain(up, out))
		neighborList.push_front(up);

	PathmapTile* down = world->GetTile(startTile->x, startTile->y + 1);
	if (down && !down->isVisitedFlag && !down->isBlockingFlag && ListDoesNotContain(down, out))
		neighborList.push_front(down);

	PathmapTile* right = world->GetTile(startTile->x + 1, startTile->y);
	if (right && !right->isVisitedFlag && !right->isBlockingFlag && ListDoesNotContain(right, out))
		neighborList.push_front(right);

	PathmapTile* left = world->GetTile(startTile->x - 1, startTile->y);
	if (left && !left->isVisitedFlag && !left->isBlockingFlag && ListDoesNotContain(left, out))
		neighborList.push_front(left);

	neighborList.sort(SortFromGhostSpawn);

	for (auto& tile : neighborList)
	{
		out.push_back(tile);

		if (Pathfind(world, tile, endTile, out))
			return true;

		out.pop_back();
	}

	return false;
}
