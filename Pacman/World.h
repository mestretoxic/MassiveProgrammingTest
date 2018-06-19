#ifndef WORLD_H
#define WORLD_H

#include <memory>
#include "Vector2.h"
#include "PathmapTile.h"
#include "Gate.h"
#include "Timer.h"
#include "Avatar.h"
#include "Config.h"

class Pacman;
class GameEntity;
class Drawer;
class Dot;
class BigDot;
class Ghost;

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
	void GetPath(const Ghost* ghost, const Vector2i& to, bool ignoreSpawn, std::list<PathmapTile*>& out);
	void GetNextValidTile(const Vector2i& direction, Vector2i& out) const;
	bool HasDots() const;

	PathmapTile* GetIntersectedTile(const GameEntity* entity) const;
	PathmapTile* GetTile(int x, int y) const;
	Ghost* GetGhostAt(int x, int y) const;
	void SetPowerUpActive(bool value);
	void SetAvatarMovement(Direction movement);
	Vector2i GetAvatarPosition() const;
	void GetFarthestTileFromAvatar(Vector2i& to) const;

private:
	std::vector<PathmapTile*> m_pathmapTiles;
	std::vector<Ghost*> m_ghosts;
	std::unique_ptr<Gate> m_gateLeft;
	std::unique_ptr<Gate> m_gateRight;
	std::unique_ptr<Avatar> m_avatar;
	Timer m_powerUpTimer;
	Timer m_resetTimer;
	Vector2i m_avatarStartPosition;
	Vector2i m_mapSize;
};

#endif // WORLD_H