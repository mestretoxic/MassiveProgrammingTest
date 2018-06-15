#pragma once
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <SDL_stdinc.h>

#define VAR_NAME(name) #name

typedef std::map<std::string, std::string> NameToValueMap;

class Config
{
public:
	static int tileSize;
	static const char* fontMain;
	static const char* fontHud;

	static bool ReadConfig(const char* fileName);
};

int Config::tileSize;
const char* Config::fontMain;
const char* Config::fontHud;

inline bool Config::ReadConfig(const char* fileName)
{
	NameToValueMap vars;

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
			assert(splitAt != std::string::npos && "Malformed config file!");
			const auto varName = line.substr(0, splitAt);
			vars[varName] = line.substr(splitAt + 3);
		}
		file.close();
	}

	tileSize = SDL_atoi(vars.at(VAR_NAME(tileSize)).c_str());
	fontMain = vars.at(VAR_NAME(fontMain)).c_str();
	fontHud = vars.at(VAR_NAME(fontHud)).c_str();

	return true;
}
