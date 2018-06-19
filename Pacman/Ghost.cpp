#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"

Ghost::Ghost(const GhostType type, const Vector2i& position, const float spawnDelay)
: MovableGameEntity(position, "ghost_32.png")
, m_type(type)
, m_state(DEFAULT)
, m_pathUpdateTimer(Timer(1.0f))
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
	{
		m_state = VULNERABLE;
		m_path.clear();
	}
	else if (!value && m_state == VULNERABLE)
		m_state = DEFAULT;
}

void Ghost::Reset()
{
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
	ASSERT(world, "World is null!");

	Vector2i to(0,0);
	bool ignoreSpawn = true;

	if (!IsDead() && !IsVulnerable() && world->GetTile(GetX(), GetY())->IsSpawnTile()) // exit spawn point
	{
		ignoreSpawn = false;
		to.Set(13, 10);
	}
	else if (IsDead()) // return to spawn point
	{
		ignoreSpawn = false;
		to.Set(Config::ghostSpawnX, Config::ghostSpawnY);
	}
	else if (IsVulnerable()) // run away
	{
		if (!m_path.empty()) 
			return;

		ignoreSpawn = false;
		world->GetFarthestTileFromAvatar(to);
		if (to == GetTilePosition()) // already there
			to.Set(Config::ghostSpawnX, Config::ghostSpawnY); // go to spawn point
	}
	else // chase mode
	{
		to = world->GetAvatarPosition();
		if (!m_path.empty()) {
			const Vector2i pathTarget = Vector2i(m_path.back()->GetX(), m_path.back()->GetY());
			if ((pathTarget - to).Length() < 5)
				to = pathTarget;
		}
	}

	if (!m_path.empty() && m_path.back()->GetX() == to.x && m_path.back()->GetY() == to.y) // destination point hasn't changed
		return;

	m_path.clear();
	world->GetPath(this, to, ignoreSpawn, m_path);
}

void Ghost::Update(const float dt, World* world)
{
	ASSERT(world, "World is null!");

	m_spawnTimer.Update(dt);

	if (m_spawnTimer.isRunning)
		return;

	m_pathUpdateTimer.Update(dt);

	if (IsDead() && GetX() == Config::ghostSpawnX && GetY() == Config::ghostSpawnY)
		Reset();

	bool updatePath = false;
	if (m_pathUpdateTimer.isComplete)
	{
		updatePath = true;
		m_pathUpdateTimer.Restart();
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

	Move(speed * dt); //using super-class method
}

void Ghost::Draw(Drawer* drawer)
{
	ASSERT(drawer, "Drawer is null!");

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
	//for(auto& tile : m_path)
	//	drawer->Draw(GHOST_DEFAULT, Config::worldOffsetX + tile->GetX() * Config::tileSize, Config::worldOffsetY + tile->GetY() * Config::tileSize);
}