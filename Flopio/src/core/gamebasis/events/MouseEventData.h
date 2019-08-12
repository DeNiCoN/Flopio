#pragma once
#include "../EventManager.h"
#include "../../math/linearmath.h"

namespace engine
{
	class MouseClickEventData : public EventData
	{
	public:
		const vec2 windowCoordinates;
	        const vec2 worldCoordinates;
		MouseClickEventData(const vec2 windowCoordinates, const vec2 worldCoordinates) : windowCoordinates(windowCoordinates), worldCoordinates(worldCoordinates) {}
	};
	
	class MouseScrollEventData : public EventData
	{
	public:
		const double xoffset;
		const double yoffset;
		MouseScrollEventData(const double xoffset, const double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
	};
}
