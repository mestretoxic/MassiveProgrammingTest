#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"
#include "Config.h"

Ghost::Ghost(const GhostType type, const Vector2i& position, float spawnDelay)
: MovableGameEntity(position, "ghost_32.png")
, m_type(type)
, m_state(DEFAULT)
, m_pathUpdatePeriod(0.5f)
, m_pathUpdateElapsed(m_pathUpdatePeriod)
, m_spawnTimer(spawnDelay)
, m_startPosition(position)
{
}

Ghost::~Ghost()
{
	m_path.clear();
}

void Ghost::SetVulnerable(const bool value)
{
	if (value && m_state == DEFAULT)
		m_state = VULNERABLE;
	else if (!value && m_state == VULNERABLE)
		m_state = DEFAULT;
}

void Ghost::Reset()
{
	m_state = DEFAULT;
	m_nextTileX = GetCurrentTileX();
	m_nextTileY = GetCurrentTileY();
}

void Ghost::Die(World*)
{
	m_state = DEAD;
}

void Ghost::UpdatePathfinding(World* world)
{
	Vector2i to(0,0);
	bool ignoreSpawn = true;

	const Vector2i currentPos(GetCurrentTileX(), GetCurrentTileY());
	if (!IsDead() && !IsVulnerable() && world->GetTile(GetCurrentTileX(), GetCurrentTileY())->IsSpawnTile())
	{
		ignoreSpawn = false;
		to = Vector2i(13, 10);
	}
	else if (IsDead()) 
	{
		ignoreSpawn = false;
		to = m_startPosition;
	}
	else if (IsVulnerable())
	{
		ignoreSpawn = false;
		to = Vector2i(13, 13);
	}
	else 
	{
		world->GetAvatarPosition(to);
		if (!m_path.empty()) {
			const Vector2i pathTarget = Vector2i(m_path.back()->GetX(), m_path.back()->GetY());
		if ((pathTarget - to).Length() < 5)
			to = pathTarget;
		}
	}

	if (!m_path.empty() && m_path.back()->GetX() == to.x && m_path.back()->GetY() == to.y)
		return;

	m_path.clear();
	world->GetPath(currentPos, to, ignoreSpawn, m_path);
}

void Ghost::Update(const float dt, World* world)
{
	static float elapsed = 0.f;
	elapsed += dt;
	if (elapsed < m_spawnTimer)
		return;

	if (GetCurrentTileX() == m_startPosition.x && GetCurrentTileY() == m_startPosition.y)
		Reset();

	bool updatePath = false;
	m_pathUpdateElapsed -= dt;
	if (m_pathUpdateElapsed <= 0.f) {
		updatePath = true;
		m_pathUpdateElapsed = m_pathUpdatePeriod;
	}

	float speed = 0.f;
	switch (m_state)
	{
		case VULNERABLE: 
			speed = Config::ghostVulnerableVelocity;
			break;
		case DEAD:
			speed = Config::ghostDeadVelocity;
			break;
		default:
			speed = Config::ghostVelocity;
	}

	if (IsAtDestination() || updatePath)
	{
		UpdatePathfinding(world);
		if (!m_path.empty() && IsInTileCenter())
		{
			PathmapTile* nextTile = m_path.front();
			m_path.pop_front();
			SetNextTile(nextTile->GetX(), nextTile->GetY());
		}
	}

	const Vector2f destination(float(m_nextTileX * Config::tileSize), float(m_nextTileY * Config::tileSize));
	Vector2f direction = destination - m_position;

	const float distanceToMove = dt * speed;
	if (distanceToMove > direction.Length())
	{
		m_position = destination;
	}
	else
	{
		direction.Normalize();
		m_position += direction * distanceToMove;
	}
}

void Ghost::Draw(Drawer* drawer)
{
	switch(m_state)
	{
		case DEAD:
			m_image = GHOST_DEAD;
			break;
		case VULNERABLE:
			m_image = GHOST_VULNERABLE;
			break;
		default:
			switch(m_type)
			{
			case BLINKY:
				m_image = GHOST_BLINKY;
				break;
			case PINKY:
				m_image = GHOST_PINKY;
				break;
			case CLYDE:
				m_image = GHOST_CLYDE;
				break;
			case INKY:
				m_image = GHOST_INKY;
				break;
			default:
				m_image = GHOST_DEFAULT;
			}
	}

	GameEntity::Draw(drawer); //using super-class method

	//DEBUG draw ghost's path
	//for(auto& tile : m_path) {
	//	drawer->Draw(GHOST_DEFAULT, Config::worldOffsetX + tile->GetX() * Config::tileSize, Config::worldOffsetY + tile->GetY() * Config::tileSize);
	//}
}