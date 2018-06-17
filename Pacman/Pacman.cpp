#include "Pacman.h"
#include "Drawer.h"
#include "SDL.h"
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"
#include "Config.h"

Pacman* Pacman::Create(Drawer* drawer)
{
	auto* pacman = new Pacman(drawer);

	if (!pacman->Init())
	{
		delete pacman;
		pacman = nullptr;
	}

	return pacman;
}

Pacman::Pacman(Drawer* drawer)
: m_lives(0)
, m_score(0)
, m_fps(0)
, m_drawer(drawer)
{
	m_world = new World();
}

Pacman::~Pacman() {
	SAFE_DELETE(m_world);
}

bool Pacman::Init()
{
	Config::ReadConfig("config.txt");
	m_lives = Config::lives;
	m_world->Init();

	return true;
}

bool Pacman::Update(const float dt, SDL_Event& event)
{
	if (!UpdateInput(event))
		return false;

	if (CheckEndGameCondition())
		return true;

	m_world->Update(dt, this);

	if (dt > 0)
		m_fps = static_cast<int>(1 / dt);

	return true;
}

bool Pacman::UpdateInput(SDL_Event& event) const
{
	if (event.type == SDL_KEYDOWN) {
		switch(event.key.keysym.sym)
		{
			case SDLK_ESCAPE:
				return false;

			case SDLK_UP:
				m_world->SetAvatarMovement(Movement::UP);
				break;
			case SDLK_DOWN:
				m_world->SetAvatarMovement(Movement::DOWN);
				break;
			case SDLK_RIGHT:
				m_world->SetAvatarMovement(Movement::RIGHT);
				break;
			case SDLK_LEFT:
				m_world->SetAvatarMovement(Movement::LEFT);
				break;
		default: ;
		}
	}

	return true;
}

bool Pacman::CheckEndGameCondition() const
{
	return !m_world->HasDots() || m_lives <= 0;
}

bool Pacman::Draw() const
{
	m_world->Draw(m_drawer);

	const char* fontPath = Config::fontHud.c_str();
	int size = Config::fontHudSize;

	std::string scoreString = "Score: " + std::to_string(m_score);
	m_drawer->DrawText(scoreString.c_str(), fontPath, size, 20, 50);

	std::string livesString = "Lives: " + std::to_string(m_lives);
	m_drawer->DrawText(livesString.c_str(), fontPath, size, 20, 80);

	std::string fpsString = "FPS: " + std::to_string(m_fps);
	m_drawer->DrawText(fpsString.c_str(), fontPath, size, 880, 50);

	fontPath = Config::fontMain.c_str();
	size = Config::fontMainSize;

	if (!m_world->HasDots())
	{
		m_drawer->DrawTextAligned("You win!", fontPath, size);
	}
	else if (m_lives <= 0)
	{
		m_drawer->DrawTextAligned("You lose!", fontPath, size);	
	}

	return true;
}
