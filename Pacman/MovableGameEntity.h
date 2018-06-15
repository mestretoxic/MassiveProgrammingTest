#ifndef MOVABLEGAMEENTITY_H
#define MOVABLEGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2.h"

class World;

class MovableGameEntity : public GameEntity
{
public:
	MovableGameEntity(const Vector2i& start, const char* image);

	void SetNextTile(int x, int y);
	int GetCurrentTileX() const;
	int GetCurrentTileY() const;
	bool IsAtDestination() const;
	bool IsInTileCenter() const;

protected:

	int m_nextTileX;
	int m_nextTileY;

};

#endif // MOVABLEGAMEENTITY_H