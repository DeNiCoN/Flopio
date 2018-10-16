#pragma once
#include "App.h"
#include "Logger.h"

namespace engine {

	extern App *currentApp;
	extern Logger log;
	bool appInit(App &app, AppConfig &config);
}