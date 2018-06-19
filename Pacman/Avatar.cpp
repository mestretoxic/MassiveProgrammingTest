#include "Avatar.h"
#include "Drawer.h"
#include "World.h"
#include "Config.h"

inline std::vector<const char*> GetAvatarImages(const Direction direction)
{
	switch(direction)
	{
		case UP: return AVATAR_UP;
		case LEFT: return AVATAR_LEFT;
		case DOWN: return AVATAR_DOWN;
		case RIGHT: return AVATAR_RIGHT;
		default: return AVATAR_RIGHT;
	}
}

Avatar::Avatar(const Vector2i& start)
: MovableGameEntity(start, nullptr)
, m_powerUp(false)
, m_timeLine(AVATAR_RIGHT, true, AVATAR_FPS)
, m_currentMovement(STOP)
, m_nextMovement(STOP)
{
}

void Avatar::Draw(Drawer* drawer)
{
	m_image = m_timeLine.GetImage();
	GameEntity::Draw(drawer); // using super-class method
}

void Avatar::Update(const float dt, World* world)
{
	const float speed = m_powerUp ? Config::powerupVelocity : Config::avatarVelocity;
	if (Move(dt * speed))
		m_timeLine.Update(dt);

	if (m_nextMovement != m_currentMovement && IsInTileCenter())
		SetMovement(m_nextMovement, world);
}

// assuming that avatar can change direction if it's approximately in the center of the tile
// or new direction of movement is opposite to current direction
bool Avatar::CanChangeDirection(const int startCoord, const int endCoord, const bool isOpposite) const
{
	if (isOpposite)
		return true;

	if (startCoord == endCoord)
		return false;

	return IsInTileCenter();
}

void Avatar::ChangeDirection(Vector2i& newDirection)
{
	SetNextTile(newDirection.x, newDirection.y);
	m_timeLine.Init(GetAvatarImages(m_currentMovement), true, AVATAR_FPS);
}

void Avatar::SetMovement(const Direction newMovement, World* world)
{
	bool canUpdate = false;
	const auto start = Vector2i(GetX(), GetY());
	Vector2i nextTile = start;
	switch(newMovement)
	{
	case RIGHT:
		world->GetNextValidTile(Vector2i(1, 0), nextTile);
		canUpdate = CanChangeDirection(start.x, nextTile.x, m_currentMovement == LEFT);
		break;
	case LEFT:
		world->GetNextValidTile(Vector2i(-1, 0), nextTile);
		canUpdate = CanChangeDirection(start.x, nextTile.x, m_currentMovement == RIGHT);
		break;
	case UP:
		world->GetNextValidTile(Vector2i(0, -1), nextTile);
		canUpdate = CanChangeDirection(start.y, nextTile.y, m_currentMovement == DOWN);
		break;
	case DOWN:
		world->GetNextValidTile(Vector2i(0, 1), nextTile);
		canUpdate = CanChangeDirection(start.y, nextTile.y, m_currentMovement == UP);
		break;
	default: ;
	}

	if (canUpdate) {
		m_currentMovement = m_nextMovement = newMovement;
		ChangeDirection(nextTile);
	} else
	{
		m_nextMovement = newMovement;
	}
}

void Avatar::Die()
{
	m_currentMovement = m_nextMovement = STOP;
}

void Avatar::SetPowerUp(const bool value)
{
	m_powerUp = value;
}

Direction Avatar::GetDirection() const
{
	return m_currentMovement;
}