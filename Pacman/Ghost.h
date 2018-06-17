#ifndef GHOST_H
#define GHOST_H

#include <list>
#include "MovableGameEntity.h"
#include "PathmapTile.h"

class World;

#define GHOST_DEAD "Ghost_Dead_32.png"
#define GHOST_VULNERABLE "Ghost_Vulnerable_32.png"
#define GHOST_BLINKY "ghost_32_red.png"
#define GHOST_PINKY "ghost_32_pink.png"
#define GHOST_CLYDE "ghost_32_orange.png"
#define GHOST_INKY "ghost_32_cyan.png"
#define GHOST_DEFAULT "ghost_32.png"

enum GhostType
{
	BLINKY,
	PINKY,
	CLYDE,
	INKY
};

class Ghost : public MovableGameEntity
{
public:
	Ghost(GhostType type, const Vector2i& position);

	bool IsDead() const
	{
		return m_isDead;
	}
	bool IsVulnerable() const
	{
		return m_isVulnerable;
	}
	void Die(World* world) override;
	void Update(float dt, World* world) override;
	void Draw(Drawer* drawer) override;
	void SetVulnerable(const bool value)
	{
		m_isVulnerable = value;
	}

private:
	GhostType m_type;
	int m_desiredMovementX;
	int m_desiredMovementY;
	bool m_isVulnerable;
	bool m_isDead;

	std::list<PathmapTile*> m_path;

};

#endif // GHOST_H