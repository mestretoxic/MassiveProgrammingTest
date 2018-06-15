#ifndef STATICGAMEENTITY_H
#define STATICGAMEENTITY_H

#include "GameEntity.h"
#include "Vector2.h"

class StaticGameEntity : public GameEntity
{
public:
	StaticGameEntity(Vector2i position, const char* image);
	~StaticGameEntity(void);
};

#endif // STATICGAMEENTITY_H