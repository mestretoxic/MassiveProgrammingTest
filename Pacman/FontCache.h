#pragma once
#include <SDL_ttf.h>
#include <map>

class FontCache
{
public:
		/**
	 * \brief Cache font data
	 * \param fontPath 
	 * \param fontSize 
	 * \return always valid pointer
	 */
	static TTF_Font* GetFontData(const char* fontPath, int fontSize);
private:
	static std::map<const char*, std::map<int, TTF_Font*>> m_loadedFonts;
};

