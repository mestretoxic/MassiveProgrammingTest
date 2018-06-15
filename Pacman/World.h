#ifndef WORLD_H
#define WORLD_H

#include <list>
#include "Vector2.h"
#include "PathmapTile.h"
#include "Cherry.h"
#include "Avatar.h"
#include "Ghost.h"
#include <cassert>

class Pacman;
class GameEntity;
class Drawer;
class Dot;
class BigDot;

#define WORLD_OFFSET Vector2i(220, 60);
#define TILE_SIZE 22
#define AVATAR_START_POSITION Vector2i(13, 22);
#define GHOST_START_POSITION Vector2i(13, 13);

class World
{
public:
	World(void);
	~World(void);

	void Init();
	void Update(float dt, Pacman* game);
	void Draw(Drawer* drawer) const;
	bool TileIsValid(int x, int y);
	bool HasIntersectedDot(const GameEntity* entity) const;
	bool HasIntersectedBigDot(const GameEntity* entity) const;
	bool HasIntersectedCherry(const GameEntity* entity) const;
	void GetPath(int fromX, int fromY, int toX, int toY, std::list<PathmapTile*>& out);
	void GetNextValidTile(const Vector2i& direction, Vector2i& out);
	bool HasDots() const
	{
		return !m_dots.empty() && !m_bigDots.empty();
	}

	PathmapTile* GetTile(int x, int y);

	static Vector2i GetWorldOffset()
	{
		return WORLD_OFFSET;
	}

	int GetAvatarTileX() const
	{
		assert(m_avatar && "Avatar is null");
		return m_avatar->GetCurrentTileX();
	}

	int GetAvatarTileY() const
	{
		assert(m_avatar && "Avatar is null");
		return m_avatar->GetCurrentTileY();
	}

	Avatar* GetAvatar() const
	{
		return m_avatar;
	}

	static Vector2i GetAvatarStartPosition()
	{
		return AVATAR_START_POSITION;
	}

	static Vector2i GetGhostStartPosition()
	{
		return GHOST_START_POSITION;
	}

	static int GetTileSize()
	{
		return TILE_SIZE;
	}

private:
	std::list<PathmapTile*> m_pathmapTiles;
	std::list<Dot*> m_dots;
	std::list<BigDot*> m_bigDots;
	std::list<Cherry*> m_cherry;
	std::vector<Ghost*> m_ghosts;
	Avatar* m_avatar;
	float m_ghostGhostCounter;
};

#endif // WORLD_H