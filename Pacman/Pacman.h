#ifndef PACMAN_H
#define PACMAN_H

#include <SDL.h>

class Drawer;
class Avatar;
class World;
class Ghost;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if (p) { delete p; p = nullptr; } }
#endif

class Pacman
{
public:
	static Pacman* Create(Drawer* drawer);
	~Pacman();

	bool Update(float dt, SDL_Event&);
	bool Draw() const;

	int m_lives;
	int m_score;
	int m_fps;

private:
	Pacman(Drawer* drawer);
	bool Init();
	bool UpdateInput(SDL_Event&) const;
	bool CheckEndGameCondition() const;

	Drawer* m_drawer;
	World* m_world;
};

#endif // PACMAN_H