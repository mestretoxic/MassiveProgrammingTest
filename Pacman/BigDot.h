#ifndef BIGDOT_H
#define BIGDOT_H

#include "GameEntity.h"
#include "Vector2.h"

class BigDot : public GameEntity
{
public:
	BigDot(Vector2i position) : GameEntity(position, "Big_Dot_32.png")
	{
	}

	~BigDot(void) = default;
};

#endif // BIGDOT_H