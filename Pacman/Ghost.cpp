#include "Ghost.h"
#include "World.h"
#include "PathmapTile.h"
#include "Drawer.h"
#include "Avatar.h"

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
	const Vector2i startPosition = World::GetGhostStartPosition();
	world->GetPath(GetCurrentTileX(), GetCurrentTileY(), startPosition.x, startPosition.y, m_path);
}

void Ghost::Update(const float dt, World* world)
{
	if (!m_isDead && !m_isVulnerable)
		world->GetPath(GetCurrentTileX(), GetCurrentTileY(), world->GetAvatarTileX(), world->GetAvatarTileY(), m_path);

	float speed = GHOST_VELOCITY;
	const int nextTileX = GetCurrentTileX() + m_desiredMovementX;
	const int nextTileY = GetCurrentTileY() + m_desiredMovementY;

	if (m_isDead)
		speed = GHOST_VELOCITY_DEAD;

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

			m_isDead = false;
		}
	}

	const Vector2f destination(static_cast<float>(m_nextTileX * World::GetTileSize()), static_cast<float>(m_nextTileY * World::GetTileSize()));
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
		case RED:
			m_image = GHOST_RED;
			break;
		case PINK:
			m_image = GHOST_PINK;
			break;
		case ORANGE:
			m_image = GHOST_ORANGE;
			break;
		case CYAN:
			m_image = GHOST_CYAN;
			break;
		default:
			m_image = GHOST_DEFAULT;
		}

	GameEntity::Draw(drawer); //using super-class method
}
