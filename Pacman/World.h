#ifndef WORLD_H
#define WORLD_H

#include <list>
#include "Vector2.h"
#include "PathmapTile.h"
#include "Avatar.h"

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
	void Update(float dt, Pacman* game);
	void Draw(Drawer* drawer) const;
	bool TileIsValid(int x, int y);
	bool HasIntersectedDot(const GameEntity* entity) const;
	bool HasIntersectedBigDot(const GameEntity* entity) const;
	bool HasIntersectedCherry(const GameEntity* entity) const;
	void GetPath(int fromX, int fromY, int toX, int toY, std::list<PathmapTile*>& out);
	void GetNextValidTile(const Vector2i& direction, Vector2i& out);
	bool HasDots() const;

	PathmapTile* GetTile(int x, int y);
	void SetAvatarMovement(Movement movement);
	int GetAvatarTileX() const;
	int GetAvatarTileY() const;
	Avatar* GetAvatar() const;
	Vector2i GetAvatarStartPosition() const;
	Vector2i GetGhostStartPosition() const;

private:
	std::vector<PathmapTile*> m_pathmapTiles;
	std::list<Dot*> m_dots;
	std::list<BigDot*> m_bigDots;
	Cherry* m_cherry;
	std::vector<Ghost*> m_ghosts;
	Avatar* m_avatar;
	float m_ghostGhostCounter;
	Vector2i m_avatarStartPosition;
	Vector2i m_ghostStartPosition;
};

#endif // WORLD_H