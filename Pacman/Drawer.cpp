#include "Drawer.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <cassert>

Drawer* Drawer::Create(SDL_Window* window, SDL_Renderer* renderer)
{
	auto drawer = new Drawer(window, renderer);

	if (!drawer->Init())
	{
		delete drawer;
		drawer = nullptr;
	}

	return drawer;
}

Drawer::Drawer(SDL_Window* window, SDL_Renderer* renderer)
: m_window(window)
, m_renderer(renderer)
{
}

Drawer::~Drawer()
{
	//SDL_DestroyWindow(m_window);
	//SDL_DestroyRenderer(m_renderer);
}

bool Drawer::Init() const
{
	return m_window != nullptr;
}

SurfaceData* Drawer::GetSurfaceData(const char* imagePath)
{
	const auto it = m_loadedTextures.find(imagePath);
	if (it != m_loadedTextures.end())
	{
		return it->second;
	}

	SDL_Surface* surface = IMG_Load(imagePath);
	if (!surface)
		return nullptr;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	if (!texture)
		return nullptr;

	const auto surfaceData = new SurfaceData(surface, texture);
	m_loadedTextures[imagePath] = surfaceData;
	return surfaceData;
}

void Drawer::Draw(const char* image, const int pixelX, const int pixelY)
{
	const SurfaceData* surfaceData = GetSurfaceData(image);
	SDL_Rect sizeRect;
    sizeRect.x = 0;
    sizeRect.y = 0;
    sizeRect.w = surfaceData->surface->w;
    sizeRect.h = surfaceData->surface->h;

    SDL_Rect posRect;
    posRect.x = pixelX;
    posRect.y = pixelY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(m_renderer, surfaceData->texture, &sizeRect, &posRect);	
}

void Drawer::DrawText(const char* text, const char* fontPath, const int size, const int x, const int y) const
{
	TTF_Font* font = TTF_OpenFont(fontPath, size);
	assert(font && "Font is null check config file");

	const SDL_Color fg = {255,255,0,255};
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(m_renderer, surface);

	SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = surface->w ;
    sizeRect.h = surface->h ;

    SDL_Rect posRect ;
    posRect.x = x;
    posRect.y = y;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(m_renderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}

void Drawer::DrawTextAligned(const char* text, const char* fontPath, const int fontSize) const
{
	TTF_Font* font = TTF_OpenFont(fontPath, fontSize);

	const SDL_Color fg = {255,255,0,255};
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(m_renderer, surface);

	SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = surface->w ;
    sizeRect.h = surface->h ;

    SDL_Rect posRect;
	int w, h;
	SDL_GetWindowSize(m_window, &w, &h);
    posRect.x = (w - sizeRect.w) / 2;
    posRect.y = (h - sizeRect.h) / 2;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(m_renderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}