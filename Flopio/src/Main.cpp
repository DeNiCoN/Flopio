#include "game/Flopio.h"
#include "core/Engine.h"

using namespace engine;
using namespace game;

int main() 
{
	Flopio app;
	AppConfig config;
	return appInit(app, config);
}