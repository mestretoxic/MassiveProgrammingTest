#include "GameEntity.h"
#include "Drawer.h"
#include "World.h"
#include "Config.h"

GameEntity::GameEntity(const Vector2i& position, const char* image)
: m_position(Vector2f(0.f, 0.f))
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

Vector2i GameEntity::GetTilePosition() const
{
	return Vector2i(GetX(), GetY());
}

int GameEntity::GetX() const
{
	return int(m_position.x / Config::tileSize);
}

int GameEntity::GetY() const
{
	return int(m_position.y / Config::tileSize);
}

bool GameEntity::IsInTileCenter() const
{
	return m_position.x - (GetX() * Config::tileSize) < 0.5f
			&& m_position.y - (GetY() * Config::tileSize) < 0.5f;
}