#include "Pathfinder.h"
#include "PathmapTile.h"
#include "World.h"
#include "Ghost.h"

bool ListDoesNotContain(PathmapTile* tileToFind, std::list<PathmapTile*>& list)
{
	for (auto& tile : list)
	{
		if (tile == tileToFind)
			return false;
	}
	return true;
}

PathmapTile* Pathfinder::GetValidTileAt(World* world, const Ghost* ghost, const int x, const int y, const bool ignoreSpawn)
{
	ASSERT_NULL(world, "World is null!");
	ASSERT_NULL(ghost, "Ghost is null!");

	Ghost* entity = world->GetGhostAt(x, y);
	if (entity)
		return nullptr;

	PathmapTile* tile = world->GetTile(x, y);
	
	if (!tile) 
		return nullptr;
	
	if (ignoreSpawn && tile->IsSpawnTile())
		return nullptr;

	if (ghost->IsVulnerable() && tile->GetTilePosition() == world->GetAvatarPosition())
		return nullptr;

	if (tile && !tile->m_visited && !tile->IsBlockingTile()) 
		return tile;

	return nullptr;
}

bool Pathfinder::Pathfind(World* world, const Ghost* ghost, PathmapTile* startTile, PathmapTile* endTile, const bool ignoreSpawn, std::list<PathmapTile*>& out)
{
	ASSERT_BOOL(world, "World is null!");
	ASSERT_BOOL(ghost, "Ghost is null!");

	if (!startTile || !endTile)
		return false;

	startTile->m_visited = true;

	if (startTile->IsBlockingTile())
		return false;

	if (startTile == endTile)
		return true;

	std::list<PathmapTile*> neighborList;
	PathmapTile* up = GetValidTileAt(world, ghost, startTile->GetX(), startTile->GetY() - 1, ignoreSpawn);
	if (up && ListDoesNotContain(up, out))
		neighborList.push_front(up);

	PathmapTile* down =  GetValidTileAt(world, ghost, startTile->GetX(), startTile->GetY() + 1, ignoreSpawn);
	if (down && ListDoesNotContain(down, out))
		neighborList.push_front(down);

	PathmapTile* right = GetValidTileAt(world, ghost, startTile->GetX() + 1, startTile->GetY(), ignoreSpawn);
	if (right && ListDoesNotContain(right, out))
		neighborList.push_front(right);

	PathmapTile* left = GetValidTileAt(world, ghost, startTile->GetX() - 1, startTile->GetY(), ignoreSpawn);
	if (left && ListDoesNotContain(left, out))
		neighborList.push_front(left);

	neighborList.sort([endTile](PathmapTile* a, PathmapTile* b)
	{
		return abs(a->GetX() - endTile->GetX()) + abs(a->GetY() - endTile->GetY()) < abs(b->GetX() - endTile->GetX()) + abs(b->GetY() - endTile->GetY());
	});

	for (auto& tile : neighborList)
	{
		out.push_back(tile);

		if (Pathfind(world, ghost, tile, endTile, false, out))
			return true;

		out.pop_back();
	}

	return false;
}
