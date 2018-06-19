#include "Drawer.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <cassert>
#include "FontCache.h"

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

Drawer::~Drawer() = default;

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
	assert(image && "Image is null");
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

void Drawer::DrawText(const TextParams& textParams)
{
	TTF_Font* font = m_fontCache.GetFont(textParams.fontPath, textParams.size);
	assert(font && "Font is null");

	SDL_Surface* surface = TTF_RenderText_Blended(font, textParams.text, textParams.color);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(m_renderer, surface);

	SDL_Rect sizeRect;
    sizeRect.x = 0;
    sizeRect.y = 0;
    sizeRect.w = surface->w;
    sizeRect.h = surface->h;
		
	SDL_Rect posRect;
	if (textParams.aligned) {
		int w, h;
		SDL_GetWindowSize(m_window, &w, &h);
	    posRect.x = (w - sizeRect.w) / 2;
	    posRect.y = (h - sizeRect.h) / 2;
	} else {
	    posRect.x = textParams.x;
	    posRect.y = textParams. y;
	}
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(m_renderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
}