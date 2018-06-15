#include "GameEntity.h"
#include "Drawer.h"
#include "World.h"

GameEntity::GameEntity(const Vector2i& position, const char* image)
: m_isMarkedForDeleteFlag(false)
, m_position(Vector2f(0.f, 0.f))
, m_image(image)
{
	m_position.x = position.x * 22.f;
	m_position.y = position.y * 22.f;
}

bool GameEntity::Intersect(const GameEntity* entity)
{
	return (entity->GetPosition() - m_position).Length() < 5.f;
}

void GameEntity::Draw(Drawer* drawer)
{
	const auto pixelX = static_cast<int>(m_position.x) + World::GetWorldOffset().x;
	const auto pixelY = static_cast<int>(m_position.y) + World::GetWorldOffset().y;
	drawer->Draw(m_image, pixelX, pixelY);
}

void GameEntity::Update(float, World*)
{
}
