#include "PathmapTile.h"

PathmapTile::PathmapTile(const Vector2i position, const bool isBlocking, const bool isSpawn, const bool hasDot, const bool hasBigDot)
:GameEntity(position, nullptr)
, m_isBlockingTile(isBlocking)
, m_isSpawnTile(isSpawn)
, m_hasDot(hasDot)
, m_hasBigDot(hasBigDot)
, m_hasCherry(false)
, m_x(position.x)
, m_y(position.y)
, m_visited(false)
{
}

bool operator==(const PathmapTile& lhs, const PathmapTile& rhs)
{
	return lhs.GetPosition() == rhs.GetPosition();
}

void PathmapTile::Draw(Drawer* drawer)
{
	if (m_hasDot)
		m_image = DOT_IMAGE;
	else if (m_hasBigDot)
		m_image = BIG_DOT_IMAGE;
	else
		m_image = nullptr;
	GameEntity::Draw(drawer);
}

bool PathmapTile::IsBlockingTile() const
{
	return m_isBlockingTile;
}
bool PathmapTile::IsSpawnTile() const
{
	return m_isSpawnTile;
}
int PathmapTile::GetX() const
{
	return m_x;
}
int PathmapTile::GetY() const
{
	return m_y;
}