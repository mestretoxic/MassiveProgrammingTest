#pragma once
#include <SDL_ttf.h>
#include <map>

typedef std::map<int, TTF_Font*> FontSizeMap;
typedef std::map<const char*, FontSizeMap> FontNameMap;

class FontCache
{
public:
	FontCache() = default;
	~FontCache();
	TTF_Font* GetFont(const char* fontPath, int fontSize);
private:
	FontNameMap m_loadedFonts;
};

