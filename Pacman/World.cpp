#include <fstream>
#include <string>
#include "World.h"
#include "Pacman.h"
#include "Pathfinder.h"
#include "Config.h"
#include "Ghost.h"
#include "Drawer.h"
#include "Defines.h"

template<typename T>
void CleanupContainer(T& container)
{
	for (auto& it : container) {
		SAFE_DELETE(it);
	}
	container.clear();
}

template<typename T>
void DrawEntities(T container, Drawer* drawer)
{
	for (const auto& entity : container)
		entity->Draw(drawer);
}

template <typename T, typename Pred>
typename std::vector<T>::iterator BinarySearch(std::vector<T> container, Pred pred)
{
    auto it_l = container.begin();
    auto it_r = container.end();
	while (it_l <= it_r)
    {
        auto mid = it_l + (it_r - it_l) / 2;
 
        // Check if x is present at mid
        if (pred(*mid) == 0)
            return mid;
 
        // If x greater, ignore left half
        if (pred(mid) < 0)
            it_l = mid + 1;
 
        // If x is smaller, ignore right half
        else
            it_r = mid - 1;
    }
 
    // if we reach here, then element was
    // not present
    return container.end();
}

template <typename T>
T GetEntityAt(std::vector<T> container, const int x, const int y)
{
	for (const auto& entity : container)
	{
		if (entity->GetX() == x && entity->GetY() == y)
			return entity;
	}

	return nullptr;
}

World::World()
: m_gateLeft(nullptr)
, m_gateRight(nullptr)
, m_avatar(nullptr)
, m_powerUpTimer(Timer(10.f))
, m_resetTimer(Timer(2.f))
, m_avatarStartPosition(Vector2i(0, 0))
, m_mapSize(0, 0)
{
}

World::~World()
{
	CleanupContainer(m_ghosts);
	CleanupContainer(m_pathmapTiles);
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
		while (!mapFile.eof() )
		{
			std::getline (mapFile,line);
			for (unsigned int i = 0; i < line.length(); i++)
			{
				const Vector2i position = Vector2i(i, lineIndex);
				m_pathmapTiles.push_back(
					new PathmapTile(
						position,
						line[i] == 'x',
						line[i] == '-',
						line[i] == '.',
						line[i] == 'o'
					)
				);
			
				if (line[i] == '<')
					m_gateLeft = std::make_unique<Gate>(position);

				if (line[i] == '>')
					m_gateRight = std::make_unique<Gate>(position);
			}
			lineIndex++;
		}
		mapFile.close();

		m_mapSize.Set(line.length(), lineIndex);
	}

	Reset();
}

void World::Reset()
{
	m_avatar.reset(new Avatar(m_avatarStartPosition));

	CleanupContainer(m_ghosts);
	m_ghosts.push_back(new Ghost(BLINKY, Vector2i(13, 10), 1.f));
	m_ghosts.push_back(new Ghost(INKY, Vector2i(11, 13), 2.f));
	m_ghosts.push_back(new Ghost(PINKY, Vector2i(13, 13), 3.f));
	m_ghosts.push_back(new Ghost(CLYDE, Vector2i(15, 13), 4.f));

	m_resetTimer.Reset();
	m_powerUpTimer.Reset();
}

void World::Update(const float dt, Pacman* game)
{
	ASSERT(m_avatar, "Avatar is null");

	m_resetTimer.Update(dt);
	m_powerUpTimer.Update(dt);

	if (m_resetTimer.isRunning)
		return;

	if (m_resetTimer.isComplete)
		Reset();

	m_avatar->Update(dt, this);
	
	for (const auto& ghost : m_ghosts) 
		ghost->Update(dt, this);

	if (m_avatar->Intersect(m_gateRight.get(), 0.5f) && m_avatar->GetDirection() == RIGHT)
	{
		m_avatar->SetPosition(m_gateLeft->GetTilePosition());
		m_avatar->SetMovement(RIGHT, this);
	}
	
	if (m_avatar->Intersect(m_gateLeft.get(), 0.5f) && m_avatar->GetDirection() == LEFT)
	{
		m_avatar->SetPosition(m_gateRight->GetTilePosition());
		m_avatar->SetMovement(LEFT, this);
	}

	PathmapTile* tile = GetIntersectedTile(m_avatar.get());
	if (tile)
	{
		if (tile->m_hasDot)
		{
			game->UpdateScore(Config::pointsDot);
			tile->m_hasDot = false;
		} 
		else if (tile->m_hasBigDot)
		{
			game->UpdateScore(Config::pointsBigDot);
			SetPowerUpActive(true);
			m_powerUpTimer.Restart();
			tile->m_hasBigDot = false;
		}
		else if (tile->m_hasCherry)
		{
			game->UpdateScore(Config::pointsCherry);
			tile->m_hasCherry = false;
		}
	}

	if (m_powerUpTimer.isComplete)
		SetPowerUpActive(false);

	for (const auto& ghost : m_ghosts)
	{
		if (m_avatar->Intersect(ghost))
		{
			if (ghost->IsDead())
				continue;

			if (ghost->IsVulnerable())
			{
				game->UpdateScore(50);
				ghost->Die();
			}
			else
			{
				game->UpdateLives();
				m_avatar->Die();
				m_resetTimer.Start();
			}
		}
	}
}

void World::Draw(Drawer* drawer)
{
	drawer->Draw("playfield.png");
	DrawEntities(m_pathmapTiles, drawer);
	DrawEntities(m_ghosts, drawer);
	for (const auto& entity : m_ghosts)
		entity->Draw(drawer);

	m_avatar->Draw(drawer);
}

void World::SetAvatarMovement(const Direction movement)
{
	if (m_resetTimer.isRunning)
		return;

	ASSERT(m_avatar, "Avatar is null!");
	m_avatar->SetMovement(movement, this);
}

Vector2i World::GetAvatarPosition() const
{
	return m_avatar ? m_avatar->GetTilePosition() : Vector2i(-1, -1);
}

void World::GetFarthestTileFromAvatar(Vector2i& to) const
{
	Vector2i avatarPos = GetAvatarPosition();
	std::vector<Vector2i> corners { 
		Vector2i(0, 0),
		Vector2i(m_mapSize.x - 1, 0),
		Vector2i(0, m_mapSize.y - 1),
		Vector2i(m_mapSize.x - 1, m_mapSize.y - 1)
	};

	std::sort(corners.begin(), corners.end(), [&](const Vector2i& lhs, const Vector2i& rhs)
	{
		return (avatarPos - lhs).Length() > (avatarPos - rhs).Length();
	});

	to.Set(corners.front().x, corners.front().y);
}

bool World::TileIsValid(const int x, const int y) const
{
	return std::any_of(m_pathmapTiles.cbegin(), m_pathmapTiles.cend(),
		[&](const auto& tile) { return x == tile->GetX() && y == tile->GetY() && !tile->IsBlockingTile() && !tile->IsSpawnTile(); });
}

PathmapTile* World::GetIntersectedTile(const GameEntity* entity) const
{
	for (auto& tile : m_pathmapTiles)
		if (tile->Intersect(entity))
			return tile;

	return nullptr;
}

void World::GetPath(const Ghost* ghost, const Vector2i& to, const bool ignoreSpawn, std::list<PathmapTile*>& out)
{
	for (auto& tile : m_pathmapTiles)
		tile->m_visited = false;

	Pathfinder::Pathfind(this, ghost, GetTile(ghost->GetX(), ghost->GetY()), GetTile(to.x, to.y), ignoreSpawn, out);
}

void World::GetNextValidTile(const Vector2i& direction, Vector2i& out) const
{
	if (TileIsValid(out.x + direction.x, out.y + direction.y))
	{
		out += direction;
		GetNextValidTile(direction, out);
	}
}

bool World::HasDots() const
{
	return std::any_of(m_pathmapTiles.cbegin(), m_pathmapTiles.cend(),
		[](const auto& tile) { return tile->m_hasDot || tile->m_hasBigDot; });
}

PathmapTile* World::GetTile(const int x, const int y) const
{
	return GetEntityAt(m_pathmapTiles, x, y);
}

Ghost* World::GetGhostAt(const int x, const int y) const
{
	return GetEntityAt(m_ghosts, x, y);
}

void World::SetPowerUpActive(const bool value)
{
	m_avatar->SetPowerUp(value);
	for (const auto& ghost : m_ghosts)
		ghost->SetVulnerable(value);
}