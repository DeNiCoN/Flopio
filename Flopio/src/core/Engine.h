#pragma once
#include "gamebasis/App.h"
#include "Logger.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine {

	extern App *currentApp;
	extern Logger logger;
	extern bool appInit(App &app, AppConfig &config);
}