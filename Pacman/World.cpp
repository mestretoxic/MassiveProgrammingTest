#include <fstream>
#include <string>
#include <cassert>
#include "World.h"
#include "Pacman.h"
#include "Pathfinder.h"
#include "Config.h"
#include "Ghost.h"
#include "Drawer.h"

World::World()
: m_gateLeft(Vector2i(0, 0))
, m_gateRight(Vector2i(0, 0))
, m_avatar(nullptr)
, m_powerUpTimer(0.f)
, m_resetTimer(0.f)
, m_avatarStartPosition(Vector2i(0, 0))
{
}

World::~World()
{
	SAFE_DELETE(m_avatar);
}

// Removed copy-pasted code
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
				const Vector2i position = Vector2i(i, lineIndex);
				auto* tile = new PathmapTile(
					position,
					line[i] == 'x',
					line[i] == '-',
					line[i] == '.',
					line[i] == 'o'
				);
				m_pathmapTiles.push_back(tile);
			
				if (line[i] == '<')
					m_gateLeft = Gate(position);

				if (line[i] == '>')
					m_gateRight = Gate(position);
			}
			lineIndex++;
		}
		mapFile.close();
	}

	Reset();
}

void World::Reset()
{
	SAFE_DELETE(m_avatar);
	m_avatar = new Avatar(GetAvatarStartPosition());

	m_ghosts.clear();
	m_ghosts.emplace_back(GhostType::BLINKY, Vector2i(13, 10), 1.f);
	m_ghosts.emplace_back(GhostType::INKY, Vector2i(11, 13), 2.f);
	m_ghosts.emplace_back(GhostType::PINKY, Vector2i(13, 13), 3.f);
	m_ghosts.emplace_back(GhostType::CLYDE, Vector2i(15, 13), 4.f);
}

void World::Update(const float dt, Pacman* game)
{
	if (m_resetTimer > 0.f) {
		m_resetTimer -= dt;
		if (m_resetTimer <= 0.f)
		{
			Reset();
			m_resetTimer = 0.f;
		} 
		else 
			return;
	} 

	m_avatar->Update(dt, this);
	
	for (auto& ghost : m_ghosts) 
		ghost.Update(dt, this);

	if (m_avatar->Intersect(&m_gateRight, 0.5f) && m_avatar->GetDirection() == RIGHT)
	{
		m_avatar->SetPosition(m_gateLeft.GetCurrentTilePosition());
		m_avatar->SetMovement(RIGHT, this);
	}
	
	if (m_avatar->Intersect(&m_gateLeft, 0.5f) && m_avatar->GetDirection() == LEFT)
	{
		m_avatar->SetPosition(m_gateRight.GetCurrentTilePosition());
		m_avatar->SetMovement(LEFT, this);
	}

	PathmapTile* tile = GetIntersectedTile(m_avatar);
	if (tile)
	{
		if (tile->HasDot())
		{
			game->m_score += 10;
			tile->EatDot();
		} 
		else if (tile->HasBigDot())
		{
			game->m_score += 20;
			m_powerUpTimer = 10.f;
			m_avatar->SetPowerUp(true);
			for (auto& ghost : m_ghosts)
				ghost.SetVulnerable(true);
		}
		else if (tile->HasCherry())
		{
			game->m_score += 500;
			tile->EatCherry();
		}
	}

	if (m_powerUpTimer > 0.f)
	{
		m_powerUpTimer -= dt;
		if (m_powerUpTimer <= 0.f)
		{
			m_avatar->SetPowerUp(false);
			for (auto& ghost : m_ghosts)
				ghost.SetVulnerable(false);
		}
	}

	for (auto& ghost : m_ghosts)
	{
		if (m_avatar->Intersect(&ghost))
		{
			if (ghost.IsDead())
				continue;

			if (ghost.IsVulnerable())
			{
				game->m_score += 50;
				ghost.Die();
			}
			else
			{
				game->m_lives--;
				m_avatar->Die();
				m_resetTimer = 2.5f;
			}
		}
	}
}

void World::Draw(Drawer* drawer)
{
	drawer->Draw("playfield.png");
	for (const auto& tile : m_pathmapTiles)
		tile->Draw(drawer);

	for (auto& ghost : m_ghosts)
		ghost.Draw(drawer);

	m_avatar->Draw(drawer);
}

// Tiles are sorted in m_pathmapTiles, so binary search could be more effective here
bool World::TileIsValid(const int x, const int y)
{
	for (auto& tile : m_pathmapTiles)
		if (x == tile->GetX() && y == tile->GetY() && !tile->IsBlockingTile() && !tile->IsSpawnTile())
			return true;

	return false;
}

PathmapTile* World::GetIntersectedTile(const GameEntity* entity)
{
	for (auto& tile : m_pathmapTiles)
	{
		if (tile->Intersect(entity)) {
			return tile;
		}
	}
	return nullptr;
}

void World::GetPath(const Vector2i& from, const Vector2i& to, bool ignoreSpawn, std::list<PathmapTile*>& out)
{
	for (auto& tile : m_pathmapTiles)
		tile->visited = false;

	Pathfinder::Pathfind(this, GetTile(from.x, from.y), GetTile(to.x, to.y), ignoreSpawn, out);
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
	return std::any_of(m_pathmapTiles.begin(), m_pathmapTiles.end(), 
	[](PathmapTile* tile)
	{
		return tile->HasDot() || tile->HasBigDot();
	});
}

PathmapTile* World::GetTile(const int x, const int y)
{
	for (auto& tile : m_pathmapTiles)
	{
		if (tile->GetX() == x && tile->GetY() == y)
		{
			return tile;
		}
	}
	return nullptr;
}

void World::SetAvatarMovement(const Direction movement)
{
	if (m_resetTimer > 0.f)
		return;

	if (GetAvatar()) 
		GetAvatar()->SetMovement(movement, this);
}

Avatar* World::GetAvatar() const
{
	assert(m_avatar && "Avatar is null");
	return m_avatar;
}

void World::GetAvatarPosition(Vector2i& position) const
{
	if (GetAvatar())
		position.Set(GetAvatar()->GetCurrentTileX(), GetAvatar()->GetCurrentTileY());
}

Vector2i World::GetAvatarStartPosition() const
{
	return m_avatarStartPosition;
}

GameEntity* World::GetGhostAt(int x, int y)
{
	for (auto& ghost : m_ghosts)
	{
		if (ghost.GetCurrentTileX() == x && ghost.GetCurrentTileY() == y)
			return &ghost;
	}

	return nullptr;
}