#pragma once
#include "../EventManager.h"
namespace engine
{
	class WindowResizeEventData : public EventData
	{
	public:
		const int width;
		const int height;
		WindowResizeEventData(const int width, const int height) : width(width), height(height) {}
	};
}
