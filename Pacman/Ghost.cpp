#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"
#include "Config.h"

Ghost::Ghost(const GhostType type, const Vector2i& position, const float spawnDelay)
: MovableGameEntity(position, "ghost_32.png")
, m_type(type)
, m_state(DEFAULT)
, m_pathUpdateTimer(Timer(0.5f))
, m_spawnTimer(Timer(spawnDelay))
, m_startPosition(position)
{
	m_pathUpdateTimer.Start();
	m_spawnTimer.Start();
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
	SetPosition(m_startPosition);
	m_state = DEFAULT;
	m_nextTileX = GetX();
	m_nextTileY = GetY();
	m_path.clear();
}

void Ghost::Die()
{
	m_state = DEAD;
}

void Ghost::UpdatePathfinding(World* world)
{
	Vector2i to(0,0);
	bool ignoreSpawn = true;

	const Vector2i currentPos(GetX(), GetY());
	if (!IsDead() && !IsVulnerable() && world->GetTile(GetX(), GetY())->IsSpawnTile())
	{
		ignoreSpawn = false;
		to.Set(13, 10);
	}
	else if (IsDead()) 
	{
		ignoreSpawn = false;
		to = m_startPosition;
	}
	else if (IsVulnerable())
	{
		ignoreSpawn = false;
		to.Set(13, 13);
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
	m_spawnTimer.Update(dt);
	m_pathUpdateTimer.Update(dt);

	if (m_spawnTimer.isRunning)
		return;

	if (IsDead() && GetX() == m_startPosition.x && GetY() == m_startPosition.y)
		Reset();

	bool updatePath = false;
	if (!m_pathUpdateTimer.isRunning)
	{
		updatePath = true;
		m_pathUpdateTimer.Restart();
	}

	float speed;
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

	Move(speed * dt);
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