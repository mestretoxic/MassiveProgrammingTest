#ifndef GATE_H
#define GATE_H

#include "GameEntity.h"

class Gate : public GameEntity
{
public:
	Gate(const Vector2i& position) : GameEntity(position, nullptr)
	{
	}
};

#endif