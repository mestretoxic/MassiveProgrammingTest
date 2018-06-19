#ifndef WORLD_H
#define WORLD_H

#include <list>
#include "Vector2.h"
#include "PathmapTile.h"
#include "Avatar.h"
#include "Gate.h"
#include "Timer.h"

class Pacman;
class GameEntity;
class Drawer;
class Dot;
class BigDot;
class Ghost;
class Cherry;

class World
{
public:
	World();
	~World();

	void Init();
	void Reset();
	void Update(float dt, Pacman* game);
	void Draw(Drawer* drawer);
	bool TileIsValid(int x, int y) const;
	void GetPath(const Vector2i& from, const Vector2i& to, bool ignoreSpawn, std::list<PathmapTile*>& out);
	void GetNextValidTile(const Vector2i& direction, Vector2i& out) const;
	bool HasDots() const;

	PathmapTile* GetIntersectedTile(const GameEntity* entity) const;
	PathmapTile* GetTile(int x, int y) const;
	Ghost* GetGhostAt(int x, int y) const;
	void SetPowerUpActive(bool value);
	void SetAvatarMovement(Direction movement);
	void GetAvatarPosition(Vector2i& position) const;

private:
	std::vector<PathmapTile*> m_pathmapTiles;
	std::vector<Ghost*> m_ghosts;
	Avatar* m_avatar;
	Gate m_gateLeft;
	Gate m_gateRight;
	Timer m_powerUpTimer;
	Timer m_resetTimer;
	Vector2i m_avatarStartPosition;
};

#endif // WORLD_H