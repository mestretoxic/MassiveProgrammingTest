#ifndef DRAWER_H
#define DRAWER_H

#include <map>
#include <SDL.h>
#include <SDL_render.h>
#include "FontCache.h"

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
	, texture(pTexture)
	{		
	}

	~SurfaceData() {
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}
};

struct TextParams
{
	const char* text;
	const char* fontPath;
	int size;
	int x;
	int y;
	bool aligned;
	SDL_Color color;

	TextParams() : text(""), fontPath(""), size(14), x(0), y(0), aligned(false), color{255,255,0,255} {}
};

class Drawer
{
public:
	static Drawer* Create(SDL_Window* window, SDL_Renderer* renderer);
	~Drawer();

	void Draw(const char* image, int pixelX = 0, int pixelY = 0);
	void DrawText(const TextParams& textParams);

private:
	Drawer(SDL_Window* window, SDL_Renderer* renderer);
	bool Init() const;
	
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	// Cache fonts instead of loading every frame
	FontCache m_fontCache;

	// Cache textures instead of loading them every frame
	SurfaceData* GetSurfaceData(const char* imagePath);
	std::map<const char*, SurfaceData*> m_loadedTextures;
};

#endif // DRAWER_H