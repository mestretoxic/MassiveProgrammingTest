#ifndef DRAWER_H
#define DRAWER_H
#include <map>
#include <SDL.h>
#include <SDL_render.h>
#include "Pacman.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;

struct SurfaceData
{
	SDL_Surface* surface;
	SDL_Texture* texture;

	SurfaceData(SDL_Surface* pSurface, SDL_Texture* pTexture) 
	: surface(pSurface)
	, texture(pTexture) {}

	~SurfaceData() {
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}
};

class Drawer
{
public:
	static Drawer* Create(SDL_Window* window, SDL_Renderer* renderer);
	~Drawer();

	void Draw(const char* image, int pixelX = 0, int pixelY = 0);
	void DrawText(const char* text, const char* fontPath, int size, int x, int y) const;
	void DrawTextAligned(const char* text, const char* fontPath, int fontSize) const;

private:
	Drawer(SDL_Window* window, SDL_Renderer* renderer);
	bool Init() const;
	
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	/**
	 * \brief Cache textures instead of loading them every frame
	 * \param imagePath 
	 * \return nullptr on error
	 */
	SurfaceData* GetSurfaceData(const char* imagePath);
	std::map<const char*, SurfaceData*> m_loadedTextures;
};

#endif // DRAWER_H