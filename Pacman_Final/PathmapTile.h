#ifndef PATHMAPTILE_H
#define PATHMAPTILE_H

#include "GameEntity.h"
#include "Pathfinder.h"

#define DOT_IMAGE "Small_Dot_32.png"
#define BIG_DOT_IMAGE "Big_Dot_32.png"

class PathmapTile : public GameEntity
{
	friend class World;
	friend class Pathfinder;
public:
	PathmapTile(Vector2i position, bool isBlocking, bool isSpawn, bool hasDot, bool hasBigDot);

	void Draw(Drawer* drawer) override;
	bool IsBlockingTile() const;
	bool IsSpawnTile() const;
	int GetX() const override;
	int GetY() const override;

private:
	//accessible in Pathfinder and World
	bool m_isBlockingTile;
	bool m_isSpawnTile;
	bool m_hasDot;
	bool m_hasBigDot;
	bool m_hasCherry;
	int m_x;
	int m_y;
	bool m_visited;
};

#endif // PATHMAPTILE_H