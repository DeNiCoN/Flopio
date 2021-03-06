#pragma once
#include "../memory/queueAlloc.h"
#include <string>
#include <cstring>
#include "../utils/utils.h"
#include "../utils/HashedString.hpp"
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>

namespace engine
{
    class EventData
    {
    public:
        virtual ~EventData() = default;
    };

    class Event
    {
        HashedString name;
        EventData &data;
    public:
        Event(const HashedString& name, EventData& data)
            : name(name), data(data) {}

        const char* getName() const { return name.getString(); }
        unsigned int getHashed() const { return name.getHashed(); }
        const EventData& getData() const { return data; }
    };

    class EventManager
    {
    public:
        EventManager(unsigned int allocBufferSizeInMb);
        ~EventManager();
        void registerCallback(const HashedString& eventName,
                              std::function<void(Event&)> callback);
        void removeCallback(const HashedString& eventName,
                            std::function<void(Event&)> callback);
        void processPending();

        template <class T>
        T& newEvent(const HashedString& name, T&& move)
        {
            static_assert(std::is_base_of_v<EventData, T> &&
                          "Type need to inherit from EventData");
            auto* eventp = (Event*)queueAllocAligned(&allocator, sizeof(Event) + sizeof(T), alignof(Event) < alignof(T) ? alignof(T) : alignof(Event));
            T* eventData = new ((T*)(eventp + 1)) T(move);
            new (eventp) Event(name, *eventData);

            return *eventData;
        }
    private:
        QueueAllocator allocator;
        std::unordered_map<unsigned int, std::vector<std::function<void(Event&)>>> callbacks;
    };
}
