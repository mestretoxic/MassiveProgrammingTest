#include "MovableGameEntity.h"
#include "World.h"
#include "Config.h"

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

int MovableGameEntity::GetCurrentTileX() const
{
	return int(m_position.x / 22);
}

int MovableGameEntity::GetCurrentTileY() const
{
	return int(m_position.y / 22);
}

bool MovableGameEntity::IsInTileCenter() const
{
	return m_position.x - (GetCurrentTileX() * Config::tileSize) < 0.5 
			&& m_position.y - (GetCurrentTileY() * Config::tileSize) < 0.5;
}

void MovableGameEntity::Die(World*)
{
}
