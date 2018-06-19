#ifndef PACMAN_H
#define PACMAN_H

#include <SDL.h>
#include "Config.h"

class Drawer;
class World;

class Pacman
{
public:
	static Pacman* Create(Drawer* drawer);
	~Pacman();

	bool Update(float dt, SDL_Event&);
	bool Draw() const;
	void UpdateScore(int value);
	void UpdateLives();

private:
	Pacman(Drawer* drawer);
	bool Init();
	bool UpdateInput(SDL_Event&) const;
	bool CheckEndGameCondition() const;

	Drawer* m_drawer;
	std::unique_ptr<World> m_world;

	int m_lives;
	int m_score;
	int m_fps;
};

#endif // PACMAN_H