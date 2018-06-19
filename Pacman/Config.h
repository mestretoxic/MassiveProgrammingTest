#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <map>
#include <utility>
#include "Defines.h"

#define MSG_VAR_NOT_INITIALIZED "WARNING: Variable '%s' not present in config file\n"
#define MSG_MALFORMED_CONFIG "ERROR: Malformed Config file!"

#define WARNING(name) if (vars.count(#name) == 0) { printf(MSG_VAR_NOT_INITIALIZED, #name); } else {

#define GET_INT_VAR(name) 								\
	WARNING(name)										\
	std::string value = vars.at(#name);					\
	name = std::strtol(value.c_str(), nullptr, 10);		\
}														\

#define GET_FLOAT_VAR(name) 							\
	WARNING(name)										\
	std::string value = vars.at(#name);					\
	name = std::strtof(value.c_str(), nullptr);			\
}														\

#define GET_STRING_VAR(name) 							\
	WARNING(name)										\
	name = vars.at(#name).c_str();								\
}														\

typedef std::map<std::string, std::string> NameToValueMap;

class Config
{
public:
	static const char* worldBitmap;
	static int worldOffsetX;
	static int worldOffsetY;
	static int tileSize;
	static int avatarStartX;
	static int avatarStartY;
	static int ghostSpawnX;
	static int ghostSpawnY;
	static int lives;
	static float avatarVelocity;
	static float powerupVelocity;
	static float ghostVelocity;
	static float ghostVulnerableVelocity;
	static float ghostDeadVelocity;
	static int pointsDot;
	static int pointsBigDot;
	static int pointsCherry;
	static const char* fontMain;
	static const char* fontHud;

	static int fontMainSize;
	static int fontHudSize;

	static NameToValueMap vars;

	static bool ReadConfig(const char* fileName)
	{
		std::string line;
		std::ifstream file(fileName);
		if (file.is_open())
		{
			while (!file.eof())
			{
				std::getline(file, line);
				if (line.empty() || line.at(0) == '#')
					continue;

				const auto splitAt = line.find(" = ");
				ASSERT_BOOL((splitAt != std::string::npos), MSG_MALFORMED_CONFIG);
				const auto varName = line.substr(0, splitAt);
				const auto strValue = line.substr(splitAt + 3);
				ASSERT_BOOL((!varName.empty() && !strValue.empty()), MSG_MALFORMED_CONFIG);
				vars[varName] = strValue;
			}
			file.close();
		}

		GET_STRING_VAR(worldBitmap);
		GET_INT_VAR(worldOffsetX);
		GET_INT_VAR(worldOffsetY);
		GET_INT_VAR(tileSize);
		GET_INT_VAR(avatarStartX);
		GET_INT_VAR(avatarStartY);
		GET_INT_VAR(ghostSpawnX);
		GET_INT_VAR(ghostSpawnY);
		GET_INT_VAR(lives);
		GET_FLOAT_VAR(avatarVelocity);
		GET_FLOAT_VAR(powerupVelocity);
		GET_FLOAT_VAR(ghostVelocity);
		GET_FLOAT_VAR(ghostVulnerableVelocity);
		GET_FLOAT_VAR(ghostDeadVelocity);
		GET_INT_VAR(pointsDot);
		GET_INT_VAR(pointsBigDot);
		GET_INT_VAR(pointsCherry);

		GET_STRING_VAR(fontMain);
		GET_INT_VAR(fontMainSize);
		GET_STRING_VAR(fontHud);
		GET_INT_VAR(fontHudSize);

		return true;
	}
};

#endif