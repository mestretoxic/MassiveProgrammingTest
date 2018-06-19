#include "Config.h"

#define INIT_VAR(var) decltype(var) var

NameToValueMap Config::vars;

INIT_VAR(Config::worldOffsetX);
INIT_VAR(Config::worldOffsetY);
INIT_VAR(Config::tileSize);
INIT_VAR(Config::avatarStartX);
INIT_VAR(Config::avatarStartY);
INIT_VAR(Config::ghostSpawnX);
INIT_VAR(Config::ghostSpawnY);

INIT_VAR(Config::lives);
INIT_VAR(Config::avatarVelocity);
INIT_VAR(Config::powerupVelocity);
INIT_VAR(Config::ghostVelocity);
INIT_VAR(Config::ghostVulnerableVelocity);
INIT_VAR(Config::ghostDeadVelocity);
INIT_VAR(Config::pointsDot);
INIT_VAR(Config::pointsBigDot);
INIT_VAR(Config::pointsCherry);

INIT_VAR(Config::fontMain);
INIT_VAR(Config::fontMainSize);
INIT_VAR(Config::fontHud);
INIT_VAR(Config::fontHudSize);