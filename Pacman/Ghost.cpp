#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"
#include "Config.h"

Ghost::Ghost(const GhostType type, const Vector2i& position)
: MovableGameEntity(position, "ghost_32.png")
{
	m_type = type;
	m_isVulnerable = false;
	m_isDead = false;

	m_desiredMovementX = 0;
	m_desiredMovementY = -1;
}

void Ghost::Die(World* world)
{
	m_isDead = true;
	m_path.clear();
	const Vector2i startPosition = world->GetGhostStartPosition();
	world->GetPath(GetCurrentTileX(), GetCurrentTileY(), startPosition.x, startPosition.y, m_path);
}

void Ghost::Update(const float dt, World* world)
{
	if (GetCurrentTileX() == Config::ghostStartX && GetCurrentTileY() == Config::ghostStartY)
		m_isDead = false;

	if (!m_isDead && !m_isVulnerable)
		world->GetPath(GetCurrentTileX(), GetCurrentTileY(), world->GetAvatarTileX(), world->GetAvatarTileY(), m_path);

	const float speed = m_isDead ? Config::ghostDeadVelocity : Config::ghostVelocity;
	const int nextTileX = GetCurrentTileX() + m_desiredMovementX;
	const int nextTileY = GetCurrentTileY() + m_desiredMovementY;

	if (IsAtDestination())
	{
		if (!m_path.empty())
		{
			PathmapTile* nextTile = m_path.front();
			m_path.pop_front();
			SetNextTile(nextTile->x, nextTile->y);
		}
		else if (world->TileIsValid(nextTileX, nextTileY))
		{
			SetNextTile(nextTileX, nextTileY);
		}
		else
		{
			if (m_desiredMovementX == 1)
			{
				m_desiredMovementX = 0;
				m_desiredMovementY = 1;
			} else if (m_desiredMovementY == 1)
			{
				m_desiredMovementX = -1;
				m_desiredMovementY = 0;			
			} else if (m_desiredMovementX == -1)
			{
				m_desiredMovementX = 0;
				m_desiredMovementY = -1;
			} else
			{
				m_desiredMovementX = 1;
				m_desiredMovementY = 0;
			}
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
	if (m_isDead)
		m_image = GHOST_DEAD;
	else if (m_isVulnerable)
		m_image = GHOST_VULNERABLE;
	else
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

	GameEntity::Draw(drawer); //using super-class method
}
