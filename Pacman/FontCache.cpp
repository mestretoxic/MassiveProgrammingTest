#include "FontCache.h"
#include <cassert>

inline TTF_Font* CreateFont(const char* fontPath, const int fontSize)
{
	const auto font = TTF_OpenFont(fontPath, fontSize);
	assert(font && "Invalid font path!");

	return font;
}

TTF_Font* FontCache::GetFontData(const char* fontPath, const int fontSize)
{
	const auto itFontPath = m_loadedFonts.find(fontPath);
	if (itFontPath != m_loadedFonts.end())
	{
		auto mapFontSize = itFontPath->second;
		const auto itFontSize = mapFontSize.find(fontSize);
		if (itFontSize != mapFontSize.end())
		{
			return itFontSize->second;
		} else
		{
			mapFontSize[fontSize] = CreateFont(fontPath, fontSize);
		}
	} else
	{
		m_loadedFonts[fontPath][fontSize] = CreateFont(fontPath, fontSize);
	}
	return m_loadedFonts.at(fontPath).at(fontSize);
}
