#include "EventManager.h"
#include <type_traits>
#include <algorithm>

namespace engine
{
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