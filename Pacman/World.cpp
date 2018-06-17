#include <fstream>
#include <string>
#include <cassert>
#include "World.h"
#include "Pacman.h"
#include "Pathfinder.h"
#include "Config.h"
#include "Ghost.h"
#include "Dot.h"
#include "Drawer.h"
#include "Cherry.h"

World::World()
: m_cherry(nullptr)
, m_avatar(nullptr)
, m_ghostGhostCounter(0.f)
, m_avatarStartPosition(Vector2i(0, 0))
, m_ghostStartPosition(Vector2i(0, 0))
{
}

World::~World()
{
	SAFE_DELETE(m_avatar);
}

template <typename Container>
void DrawEntities(Container list, Drawer* drawer)
{
	for (const auto& entity : list) {
		if (entity->GetIsVisible())
			entity->Draw(drawer);
	}
}

template<typename T>
bool HasIntersectedEntity(const GameEntity* lhs, T* rhs)
{
	if (!lhs || !rhs)
		return false;

	if (!rhs->GetIsVisible())
		return false;

	if (rhs->Intersect(lhs))
	{
		rhs->SetIsVisible(false);
		return true;
	}

	return false;
}

template<typename Container>
bool HasIntersectedEntityList(const GameEntity* pEntity, Container list)
{
	if (!pEntity)
		return false;

	for (auto& entity : list)
	{
		if (HasIntersectedEntity(pEntity, entity)) {
			return true;
		}
	}

	return false;
}

// Removed copy-pasted methods
void World::Init()
{
	m_avatarStartPosition.Set(Config::avatarStartX, Config::avatarStartY);
	
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

	m_avatar = new Avatar(GetAvatarStartPosition());
	m_ghosts.emplace_back(new Ghost(GhostType::BLINKY, Vector2i(11,13)));
	m_ghosts.emplace_back(new Ghost(GhostType::INKY, Vector2i(13,13)));
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
				m_avatar->Die(this);
			}
		}
	}
}

void World::Draw(Drawer* drawer) const
{
	drawer->Draw("playfield.png");
	DrawEntities(m_dots, drawer);
	DrawEntities(m_bigDots, drawer);
	DrawEntities(m_ghosts, drawer);
	m_avatar->Draw(drawer);
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
	return HasIntersectedEntityList(entity, m_dots);
}

bool World::HasIntersectedBigDot(const GameEntity* entity) const
{
	return HasIntersectedEntityList(entity, m_bigDots);
}

bool World::HasIntersectedCherry(const GameEntity* entity) const
{
	return HasIntersectedEntity(entity, m_cherry);
}

void World::GetPath(const int fromX, const int fromY, const int toX, const int toY, std::list<PathmapTile*>& out)
{
	for (auto& tile : m_pathmapTiles) 
		tile->isVisitedFlag = false;

	Pathfinder::Pathfind(this, GetTile(fromX, fromY), GetTile(toX, toY), out);
}

void World::GetNextValidTile(const Vector2i& direction, Vector2i& out)
{
	if (TileIsValid(out.x + direction.x, out.y + direction.y))
	{
		out += direction;
		GetNextValidTile(direction, out);
	}
}

bool World::HasDots() const
{
	return !m_dots.empty() && !m_bigDots.empty();
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

void World::SetAvatarMovement(const Movement movement)
{
	if (GetAvatar()) 
		GetAvatar()->SetMovement(movement, this);
}

int World::GetAvatarTileX() const
{
	assert(m_avatar && "Avatar is null");
	return m_avatar ? m_avatar->GetCurrentTileX() : -1;
}

int World::GetAvatarTileY() const
{
	assert(m_avatar && "Avatar is null");
	return m_avatar ? m_avatar->GetCurrentTileY() : -1;
}

Avatar* World::GetAvatar() const
{
	assert(m_avatar && "Avatar is null");
	return m_avatar;
}

Vector2i World::GetAvatarStartPosition() const
{
	return m_avatarStartPosition;
}

Vector2i World::GetGhostStartPosition() const
{
	return m_ghostStartPosition;
}