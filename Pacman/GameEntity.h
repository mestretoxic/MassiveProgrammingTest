#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2.h"

class World;
class Drawer;

class GameEntity
{
public:
	GameEntity(const Vector2i& position, const char* image);
	virtual ~GameEntity() = default;

	Vector2f GetPosition() const
	{
		return m_position;
	}

	void SetPosition(const Vector2i& position);
	Vector2i GetCurrentTilePosition() const;
	int GetCurrentTileX() const;
	int GetCurrentTileY() const;
	bool IsInTileCenter() const;

	virtual bool Intersect(const GameEntity* entity, const float tolerance = 10.f);
	virtual void Draw(Drawer* drawer);
	virtual void Update(float dt, World* world);

	void SetIsVisible(bool value)
	{
		m_isVisible = value;
	}
	bool GetIsVisible() const
	{
		return m_isVisible;
	}

protected:

	bool m_isVisible;
	Vector2f m_position;
	const char* m_image;
};

#endif // GAMEENTITY_H