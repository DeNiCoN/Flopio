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
			for (auto callback : callbacks[e->getHashed()])
			{
				callback(*e);
			}
			queueFree(&allocator);
		}
	}
	//	template<class T>
//	T& EventManager::newEvent(const char* name)
//	{
//		static_assert(std::is_base_of_v<EventData, T> && "Type need to inherit from EventData");
//		Event* eventp = (Event*) queueAllocAligned(&allocator, sizeof(Event) + sizeof(T), alignof(Event) < alignof(T) ? alignof(T) : alignof(Event));
//		T* eventData = new ((T*)(eventp++)) T();
//		new (eventp) Event(name, *eventData);
//
//		return *eventData;
//	}
}