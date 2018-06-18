#include "MovableGameEntity.h"
#include "World.h"

MovableGameEntity::MovableGameEntity(const Vector2i& start, const char* image)
: GameEntity(start, image)
{
	m_nextTileX = GetCurrentTileX();
	m_nextTileY = GetCurrentTileY();
}

bool MovableGameEntity::IsAtDestination() const
{
	return GetCurrentTileX() == m_nextTileX && GetCurrentTileY() == m_nextTileY;
}

void MovableGameEntity::SetNextTile(const int x, const int y)
{
	m_nextTileX = x;
	m_nextTileY = y;
}
