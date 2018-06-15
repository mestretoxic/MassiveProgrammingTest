#ifndef PACMAN_H
#define PACMAN_H

#include <SDL.h>

struct SDL_Surface;
class Drawer;
class Avatar;
class World;
class Ghost;

#define FONT "freefont-ttf\\sfd\\FreeMono.ttf"

class Pacman
{
public:
	static Pacman* Create(Drawer* drawer);
	~Pacman(void);

	bool Update(float dt, SDL_Event&);
	bool Draw() const;

	int m_lives;
	int m_score;

private:
	explicit Pacman(Drawer* aDrawer);
	bool Init() const;
	bool UpdateInput(SDL_Event&) const;
	bool CheckEndGameCondition() const;

	Drawer* m_drawer;

	int m_fps;

	World* m_world;
};

#endif // PACMAN_H