#include "Avatar.h"
#include "Drawer.h"
#include "World.h"

inline std::vector<const char*> GetAvatarImages(const Movement direction)
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
	: MovableGameEntity(start, "open_32.png"), 
	m_currentMovement(STOP), m_nextMovement(STOP)
{
	m_timeLine = new TimeLine(AVATAR_RIGHT, true, AVATAR_FPS);
}

void Avatar::Draw(Drawer* drawer)
{
	m_image = m_timeLine->GetImage();
	GameEntity::Draw(drawer); // using super-class method
}

void Avatar::Update(const float dt, World* world)
{
	const Vector2f destination(float(m_nextTileX * World::GetTileSize()), float(m_nextTileY * World::GetTileSize()));
	Vector2f direction = destination - m_position;

	const float distanceToMove = dt * AVATAR_VELOCITY;

	if (distanceToMove > direction.Length())
	{
		m_position = destination;
	}
	else
	{
		direction.Normalize();
		m_position += direction * distanceToMove;
		m_timeLine->Update(dt);
	}

	if (m_nextMovement != m_currentMovement && IsInTileCenter())
	{
		SetMovement(m_nextMovement, world);
	}
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
	m_timeLine->Init(GetAvatarImages(m_currentMovement), true, AVATAR_FPS);
}

void Avatar::SetMovement(const Movement newMovement, World* world)
{
	bool canUpdate = false;
	const auto start = Vector2i(GetCurrentTileX(), GetCurrentTileY());
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
	SetPosition(World::GetAvatarStartPosition());
	SetNextTile(13, 22);
	m_currentMovement = m_nextMovement = STOP;
}
