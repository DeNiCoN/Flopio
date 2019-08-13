#include "EventManager.h"
#include <type_traits>
#include <algorithm>

namespace engine
{
	EventManager::EventManager(unsigned int allocBufferSizeInMb)
	{
		queueAllocInit(&allocator, new char[allocBufferSizeInMb * 1024 * 1024], allocBufferSizeInMb * 1024 * 1024);
	}

	EventManager::~EventManager()
	{
		delete[] allocator.buffer;
	}

	void EventManager::registerCallback(const char* eventName, std::function<void(Event&)> callback)
	{
		callbacks[hash(eventName, strlen(eventName))].push_back(callback);
	}

	void EventManager::processPending()
	{
		while (Event* e = (Event*)queueAllocGetFirst(&allocator))
		{
			std::cout << e->getName() << "\n";
			for (auto callback : callbacks[e->getHashed()])
			{
				callback(*e);
			}
			queueFree(&allocator);
		}
	}
}
