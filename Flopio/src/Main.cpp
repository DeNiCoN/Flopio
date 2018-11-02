#include "game/Flopio.h"
#include "core/Engine.h"
#include "core/resourse/ResourceCache.h"
#include <iostream>

using namespace engine;
using namespace game;

int main() 
{
	std::cout << sizeof(ResourceHandle);

	PoolAllocator pool;
	poolInit(&pool, new char[sizeof(ResourceHandle) * 4], sizeof(ResourceHandle), 4);

	for (int i = 0; i < 5; i++) {
		poolAlloc(&pool);
	}
	/*Flopio app;
	AppConfig config;
	return appInit(app, config);*/
}