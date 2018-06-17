#include "GameEntity.h"
#include "Drawer.h"
#include "World.h"
#include "Config.h"

GameEntity::GameEntity(const Vector2i& position, const char* image)
: m_isVisible(true)
, m_position(Vector2f(0.f, 0.f))
, m_image(image)
{
	SetPosition(position);
}

void GameEntity::SetPosition(const Vector2i& position)
{
	m_position.x = position.x * static_cast<float>(Config::tileSize);
	m_position.y = position.y * static_cast<float>(Config::tileSize);
}

bool GameEntity::Intersect(const GameEntity* entity, const float tolerance)
{
	return (entity->GetPosition() - m_position).Length() < tolerance;
}

void GameEntity::Draw(Drawer* drawer)
{
	if (!m_image)
		return;

	const auto pixelX = static_cast<int>(m_position.x) + Config::worldOffsetX;
	const auto pixelY = static_cast<int>(m_position.y) + Config::worldOffsetY;
	drawer->Draw(m_image, pixelX, pixelY);
}

void GameEntity::Update(float, World*)
{
}

Vector2i GameEntity::GetCurrentTilePosition() const
{
	return Vector2i(GetCurrentTileX(), GetCurrentTileY());
}

int GameEntity::GetCurrentTileX() const
{
	return int(m_position.x / 22);
}

int GameEntity::GetCurrentTileY() const
{
	return int(m_position.y / 22);
}

bool GameEntity::IsInTileCenter() const
{
	return m_position.x - (GetCurrentTileX() * Config::tileSize) < 0.5 
			&& m_position.y - (GetCurrentTileY() * Config::tileSize) < 0.5;
}