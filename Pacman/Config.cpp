#include "Config.h"

#define INIT_VAR(var) decltype(var) var

INIT_VAR(Config::worldOffsetX);
INIT_VAR(Config::worldOffsetY);
INIT_VAR(Config::tileSize);
INIT_VAR(Config::avatarStartX);
INIT_VAR(Config::avatarStartY);
INIT_VAR(Config::ghostStartX);
INIT_VAR(Config::ghostStartY);

INIT_VAR(Config::lives);
INIT_VAR(Config::avatarVelocity);
INIT_VAR(Config::powerupVelocity);
INIT_VAR(Config::ghostVelocity);
INIT_VAR(Config::ghostDeadVelocity);

INIT_VAR(Config::fontMain);
INIT_VAR(Config::fontMainSize);
INIT_VAR(Config::fontHud);
INIT_VAR(Config::fontHudSize);