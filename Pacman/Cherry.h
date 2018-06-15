#ifndef CHERRY_H
#define CHERRY_H

#include "GameEntity.h"
#include "Vector2.h"

class Cherry : public GameEntity
{
public:
	Cherry(Vector2i position) : GameEntity(position, "dot.png")
	{
	}

	~Cherry(void) = default;
};

#endif // CHERRY_H