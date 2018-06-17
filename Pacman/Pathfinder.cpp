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

PathmapTile* Pathfinder::GetValidTileAt(World* world, const int x, const int y, bool ignoreSpawn)
{
	GameEntity* entity = world->GetGhostAt(x, y);
	if (entity)
	{
		return nullptr;
	}
	PathmapTile* tile = world->GetTile(x, y);
	if (tile && ignoreSpawn && tile->IsSpawnTile())
	{
		return nullptr;
	}
	if (tile && !tile->visited && !tile->IsBlockingTile()) 
	{
		return tile;
	}
	return nullptr;
}

bool Pathfinder::Pathfind(World* world, PathmapTile* startTile, PathmapTile* endTile, bool ignoreSpawn, std::list<PathmapTile*>& out)
{
	if (!startTile || !endTile)
		return false;

	startTile->visited = true;

	if (startTile->IsBlockingTile())
		return false;

	if (startTile == endTile)
		return true;

	std::list<PathmapTile*> neighborList;
	PathmapTile* up = GetValidTileAt(world, startTile->GetX(), startTile->GetY() - 1, ignoreSpawn);
	if (up && ListDoesNotContain(up, out))
		neighborList.push_front(up);

	PathmapTile* down =  GetValidTileAt(world, startTile->GetX(), startTile->GetY() + 1, ignoreSpawn);
	if (down && ListDoesNotContain(down, out))
		neighborList.push_front(down);

	PathmapTile* right = GetValidTileAt(world, startTile->GetX() + 1, startTile->GetY(), ignoreSpawn);
	if (right && ListDoesNotContain(right, out))
		neighborList.push_front(right);

	PathmapTile* left = GetValidTileAt(world, startTile->GetX() - 1, startTile->GetY(), ignoreSpawn);
	if (left && ListDoesNotContain(left, out))
		neighborList.push_front(left);

	neighborList.sort([endTile](PathmapTile* a, PathmapTile* b)
	{
		return abs(a->GetX() - endTile->GetX()) + abs(a->GetY() - endTile->GetY()) < abs(b->GetX() - endTile->GetX()) + abs(b->GetY() - endTile->GetY());
	});

	for (auto& tile : neighborList)
	{
		out.push_back(tile);

		if (Pathfind(world, tile, endTile, false, out))
			return true;

		out.pop_back();
	}

	return false;
}
