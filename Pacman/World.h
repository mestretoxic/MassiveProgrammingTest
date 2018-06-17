#ifndef WORLD_H
#define WORLD_H

#include <list>
#include "Vector2.h"
#include "PathmapTile.h"
#include "Avatar.h"
#include "Gate.h"

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
	void Draw(Drawer* drawer) const;
	bool TileIsValid(int x, int y);
	bool HasIntersectedDot(const Avatar* entity);
	bool HasIntersectedBigDot(const Avatar* entity);
	bool HasIntersectedCherry(const Avatar* entity);
	void GetPath(const Vector2i& from, const Vector2i& to, bool ignoreSpawn, std::list<PathmapTile*>& out);
	void GetNextValidTile(const Vector2i& direction, Vector2i& out);
	bool HasDots() const;

	PathmapTile* GetTile(int x, int y);
	GameEntity* GetGhostAt(int x, int y);
	void SetAvatarMovement(Direction movement);
	Avatar* GetAvatar() const;
	void GetAvatarPosition(Vector2i& position) const;
	Vector2i GetAvatarStartPosition() const;

private:
	std::vector<PathmapTile*> m_pathmapTiles;
	std::vector<Ghost*> m_ghosts;
	Gate* m_gateLeft;
	Gate* m_gateRight;
	Avatar* m_avatar;
	float m_vulnerableTimer;
	float m_resetTimer;
	Vector2i m_avatarStartPosition;
};

#endif // WORLD_H