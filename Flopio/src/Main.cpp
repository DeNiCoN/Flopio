#include "core/Engine.h"
#include "game/Flopio.h"
#include "core/resourse/ResourceCache.h"
#include <iostream>

using namespace engine;
using namespace game;

int main()
{
	Flopio app;
	AppConfig config;
	return appInit(app, config);
}