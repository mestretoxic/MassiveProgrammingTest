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

enum GhostState
{
	DEFAULT,
	VULNERABLE,
	DEAD
};

class Ghost : public MovableGameEntity
{
public:
	Ghost(GhostType type, const Vector2i& position, float spawnDelay);
	~Ghost();

	bool IsDead() const
	{
		return m_state == DEAD;
	}
	bool IsVulnerable() const
	{
		return m_state == VULNERABLE;
	}

	void SetVulnerable(const bool value);
	void Reset();
	void Die(World* world) override;
	void UpdatePathfinding(World* world);
	void Update(float dt, World* world) override;
	void Draw(Drawer* drawer) override;

private:
	GhostType m_type;
	GhostState m_state;

	std::list<PathmapTile*> m_path;
	float m_pathUpdatePeriod; //ms
	float m_pathUpdateElapsed;
	float m_spawnTimer;
	Vector2i m_startPosition;
};

#endif // GHOST_H