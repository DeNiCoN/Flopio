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
		const int button;
		const int action;
		const int mods;
		MouseClickEventData(const vec2 windowCoordinates,
			       	const vec2 worldCoordinates,
				const int button, const int action,
				const int mods):
			windowCoordinates(windowCoordinates),
		       	worldCoordinates(worldCoordinates),
	       		button(button), action(action), mods(mods) {}
	};
	
	class MouseScrollEventData : public EventData
	{
	public:
		const double xoffset;
		const double yoffset;
		MouseScrollEventData(const double xoffset, const double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
	};
}
