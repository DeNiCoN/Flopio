#include "core/Engine.h"
#include "core/resourse/ResourceCache.h"
#include "game/Flopio.h"
#include <iostream>

using namespace engine;
using namespace game;

int main()
{
	std::cout << _CrtCheckMemory();
	Flopio app;
	AppConfig config;
	return appInit(app, config);
}