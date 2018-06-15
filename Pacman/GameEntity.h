#ifndef GAMEENTITY_H
#define GAMEENTITY_H

#include "Vector2.h"

class World;
class Drawer;

class GameEntity
{
public:
	GameEntity(const Vector2i& position, const char* image);
	virtual ~GameEntity(void) = default;

	Vector2f GetPosition() const
	{
		return m_position;
	}
	void SetPosition(const Vector2f& position)
	{
		m_position = position;
	}
	void SetPosition(const Vector2i& position)
	{ 	
		m_position.x = position.x * 22.f;
		m_position.y = position.y * 22.f;
	}

	virtual bool Intersect(const GameEntity* entity);
	virtual void Draw(Drawer* drawer);
	virtual void Update(float dt, World* world);

	void MarkForDelete()
	{
		m_isMarkedForDeleteFlag = true;
	}
	bool IsMarkedForDelete() const
	{
		return m_isMarkedForDeleteFlag;
	}

protected:

	bool m_isMarkedForDeleteFlag;
	Vector2f m_position;
	const char* m_image;
};

#endif // GAMEENTITY_H