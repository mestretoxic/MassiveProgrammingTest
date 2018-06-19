#include "MovableGameEntity.h"
#include "World.h"
#include "Config.h"

MovableGameEntity::MovableGameEntity(const Vector2i& start, const char* image)
: GameEntity(start, image)
{
	m_nextTileX = GetX();
	m_nextTileY = GetY();
}

bool MovableGameEntity::IsAtDestination() const
{
	return GetX() == m_nextTileX && GetY() == m_nextTileY;
}

bool MovableGameEntity::Move(const float distanceToMove)
{
	const Vector2f destination(float(m_nextTileX * Config::tileSize), float(m_nextTileY * Config::tileSize));
	Vector2f direction = destination - m_position;

	if (distanceToMove > direction.Length())
	{
		m_position = destination;
	}
	else
	{
		direction.Normalize();
		m_position += direction * distanceToMove;
		return true;
	}

	return false;
}

void MovableGameEntity::SetNextTile(const int x, const int y)
{
	m_nextTileX = x;
	m_nextTileY = y;
}
