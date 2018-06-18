#include "FontCache.h"
#include <cassert>

FontCache::~FontCache()
{
	for (auto& it : m_loadedFonts)
	{
		FontSizeMap& fontMap = it.second;
		for (auto& jt : fontMap) {
			TTF_CloseFont(jt.second);
		}
	}
	m_loadedFonts.clear();
}

TTF_Font* FontCache::GetFont(const char* fontPath, const int fontSize)
{
	assert(fontPath && "Invalid font path!");

	TTF_Font* font = nullptr;
	const auto fontMapIt = m_loadedFonts.find(fontPath);
	if (fontMapIt != m_loadedFonts.end())
	{
		auto fontSizeMap = fontMapIt->second;
		const auto fontSizeMapIt = fontSizeMap.find(fontSize);
		if (fontSizeMapIt != fontSizeMap.end())
			font = fontSizeMapIt->second;
	}

	if (!font) {
		font = TTF_OpenFont(fontPath, fontSize);
		m_loadedFonts[fontPath][fontSize] = font;
	}

	return font;
}
