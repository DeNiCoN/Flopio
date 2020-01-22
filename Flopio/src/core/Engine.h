#pragma once
#include "gamebasis/ProcessManager.h"
#include "resourse/ResourceCache.h"
#include "memory/linearAlloc.h"
#include "gamebasis/App.h"
#include "utils/Logger.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine {
    extern double frame;
    extern double render;
    extern double update;
    extern App *currentApp;
    extern Logger logger;
    extern LinearAllocator oneFrame;
    extern bool appInit(App &app, const AppConfig &config);
    extern void* oneFrameAlloc(size_t size);
}
