#ifndef AVATAR_H
#define AVATAR_H

#include "MovableGameEntity.h"
#include "Vector2.h"
#include "Timeline.h"

#define AVATAR_FPS 5
#define AVATAR_RIGHT {"open_right_32.png", "closed_right_32.png"}
#define AVATAR_LEFT {"open_left_32.png", "closed_left_32.png"}
#define AVATAR_UP {"open_up_32.png", "closed_up_32.png"}
#define AVATAR_DOWN {"open_down_32.png", "closed_down_32.png"}

enum Movement
{
	STOP,
	UP,
	LEFT,
	RIGHT,
	DOWN
};

class Avatar : public MovableGameEntity
{
public:
	Avatar(const Vector2i& start);

	void Draw(Drawer* drawer) override;
	void Update(float dt, World* world) override;
	bool CanChangeDirection(int startCoord, int endCoord, bool isOpposite) const;
	void ChangeDirection(Vector2i& newDirection);
	void SetMovement(Movement newMovement, World* world);
	void Die(World* world) override;

private:
	bool m_powerUp;
	TimeLine* m_timeLine;
	Movement m_currentMovement; // movement direction set by first directional key press
	Movement m_nextMovement; // this field stores direction from second key press, to achieve same behavior as in original game
};

#endif //AVATAR_H