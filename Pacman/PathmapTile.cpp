#include "PathmapTile.h"

PathmapTile::PathmapTile(Vector2i position, bool isBlocking, bool isSpawn, bool hasDot, bool hasBigDot):
	GameEntity(position, nullptr)
	, m_isBlockingTile(isBlocking)
	, m_isSpawnTile(isSpawn)
	, m_hasDot(hasDot)
	, m_hasBigDot(hasBigDot)
	, m_hasCherry(false)
	, m_x(position.x)
	, m_y(position.y)
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

void PathmapTile::EatDot()
{
	m_hasDot = false;
}
void PathmapTile::EatBigDot()
{
	m_hasBigDot = false;
}
void PathmapTile::EatCherry()
{
	m_hasCherry = false;
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
bool PathmapTile::HasDot() const
{
	return m_hasDot;
}
bool PathmapTile::HasBigDot() const
{
	return m_hasBigDot;
}
bool PathmapTile::HasCherry() const
{
	return m_hasCherry;
}