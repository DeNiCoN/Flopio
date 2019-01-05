#include "core/Engine.h"
#include "core/resourse/ResourceCache.h"
#include "game/Flopio.h"
#include <iostream>

using namespace engine;
using namespace game;

int main()
{
	Flopio app;
	AppConfig config;
	//config.fullscreen = true;
	return appInit(app, config);
}