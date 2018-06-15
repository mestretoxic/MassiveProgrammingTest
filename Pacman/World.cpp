#include "World.h"
#include <fstream>
#include <string>

#include "PathmapTile.h"
#include "Dot.h"
#include "BigDot.h"
#include "Drawer.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Pathfinder.h"

World::World(void)
: m_avatar(nullptr)
, m_ghostGhostCounter(0.f)
{
}

World::~World(void) = default;

template <typename T>
void DrawEntities(std::list<T> list, Drawer* drawer)
{
	for (const auto& entity : list) {
		if (!entity->IsMarkedForDelete())
			entity->Draw(drawer);
	}
}

template<typename T>
bool HasIntersectedEntity(const GameEntity* pEntity, std::list<T> list)
{
	if (!pEntity)
		return false;

	for (auto& entity : list)
	{
		if (entity->IsMarkedForDelete())
			continue;

		if (entity->Intersect(pEntity))
		{
			entity->MarkForDelete();
			return true;
		}
	}

	return false;
}

// Removed copy-pasted methods
void World::Init()
{
	std::string line;
	std::ifstream mapFile ("map.txt");
	if (mapFile.is_open())
	{
		int lineIndex = 0;
		while (! mapFile.eof() )
		{
			std::getline (mapFile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				auto* tile = new PathmapTile(i, lineIndex, (line[i] == 'x'));
				m_pathmapTiles.push_back(tile);

				if (line[i] == '.')
				{
					auto* dot = new Dot(Vector2i(i, lineIndex));
					m_dots.push_back(dot);
				} else if (line[i] == 'o')
				{
					auto* dot = new BigDot(Vector2i(i, lineIndex));
					m_bigDots.push_back(dot);
				}
			}

			lineIndex++;
		}
		mapFile.close();
	}

	m_avatar = new Avatar(Vector2i(13,22));
	m_ghosts.emplace_back(new Ghost(GhostType::RED, Vector2i(12,13)));
	m_ghosts.emplace_back(new Ghost(GhostType::CYAN, Vector2i(13,13)));
}

void World::Update(const float dt, Pacman* game)
{
	m_avatar->Update(dt, this);
	
	for (const auto& ghost : m_ghosts) ghost->Update(dt, this);

	if (HasIntersectedDot(m_avatar))
		game->m_score += 10;

	m_ghostGhostCounter -= dt;

	if (HasIntersectedBigDot(m_avatar))
	{
		game->m_score += 20;
		m_ghostGhostCounter = 20.f;

		for (const auto& ghost : m_ghosts)
			ghost->SetVulnerable(true);
	}

	if (m_ghostGhostCounter <= 0)
	{
		for (const auto& ghost : m_ghosts)
			ghost->SetVulnerable(false);
	}

	for (const auto& ghost : m_ghosts)
	{
		if (m_avatar->Intersect(ghost))
		{
			if (ghost->IsDead())
				continue;

			if (ghost->IsVulnerable())
			{
				game->m_score += 50;
				ghost->Die(this);
			}
			else
			{
				game->m_lives--;
				m_avatar->Die();
			}
		}
	}
}

void World::Draw(Drawer* drawer) const
{
	drawer->Draw("playfield.png");
	DrawEntities(m_dots, drawer);
	DrawEntities(m_bigDots, drawer);
	m_avatar->Draw(drawer);
	for (const auto& ghost : m_ghosts) ghost->Draw(drawer);
}

// Tiles are sorted in m_pathmapTiles, so binary search could be more effective here
bool World::TileIsValid(const int x, const int y)
{
	for (auto& tile : m_pathmapTiles)
		if (x == tile->x && y == tile->y && !tile->isBlockingFlag)
			return true;

	return false;
}

// Removed copy-pasted methods
bool World::HasIntersectedDot(const GameEntity* entity) const
{
	return HasIntersectedEntity(entity, m_dots);
}

bool World::HasIntersectedBigDot(const GameEntity* entity) const
{
	return HasIntersectedEntity(entity, m_bigDots);
}

bool World::HasIntersectedCherry(const GameEntity* entity) const
{
	return HasIntersectedEntity(entity, m_cherry);
}

void World::GetPath(const int fromX, const int fromY, const int toX, const int toY, std::list<PathmapTile*>& out)
{
	PathmapTile* fromTile = GetTile(fromX, fromY);
	PathmapTile* toTile = GetTile(toX, toY);
	
	for (auto& tile : m_pathmapTiles) 
		tile->isVisitedFlag = false;

	Pathfinder::Pathfind(this, fromTile, toTile, out);
}

void World::GetNextValidTile(const Vector2i& direction, Vector2i& out)
{
	if (TileIsValid(out.x + direction.x, out.y + direction.y))
	{
		out += direction;
		GetNextValidTile(direction, out);
	}
}

PathmapTile* World::GetTile(const int x, const int y)
{
	for (auto& tile : m_pathmapTiles)
	{
		if (tile->x == x && tile->x == y)
		{
			return tile;
		}
	}
	return nullptr;
}