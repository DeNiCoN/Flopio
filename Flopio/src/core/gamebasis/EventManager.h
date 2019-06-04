#include "../memory/queueAlloc.h"
#include <string>
#include "../utils.h"

namespace engine
{
	class EventData
	{
	public:
		virtual ~EventData() {}
		double b;
	};

	class Event
	{
	public:
		Event(const char* name, EventData& data)
			: name(name), hashed(hash(name, strlen(name))), data(data) {}

		const char* getName() const { return name; }
		unsigned int getHashed() const { return hashed; }
		const EventData& getData() const { return data; }
	private:
		const char* name;
		unsigned int hashed;
		EventData& data;
	};

	class EventManager
	{
	public:
		template <class T>
		T& newEvent(const char* name)
		{
			static_assert(std::is_base_of_v<EventData, T> && "Type need to inherit from EventData");
			Event* eventp = (Event*)queueAllocAligned(&allocator, sizeof(Event) + sizeof(T), alignof(Event) < alignof(T) ? alignof(T) : alignof(Event));
			T* eventData = new ((T*)(eventp++)) T();
			new (eventp) Event(name, *eventData);

			return *eventData;
		}
	private:
		QueueAllocator allocator;
	};
}