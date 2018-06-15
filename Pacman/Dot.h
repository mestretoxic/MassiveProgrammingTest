#ifndef DOT_H
#define DOT_H

#include "GameEntity.h"
#include "Vector2.h"

class Dot : public GameEntity
{
public:
	Dot(Vector2i position) : GameEntity(position, "Small_Dot_32.png")
	{
	}

	~Dot(void) = default;
};

#endif // DOT_H