#pragma once
#include "Component.h"

namespace engine
{
	class RenderComponent : public Component
	{
		virtual void VRender(const double ndelay) = 0;
	};
}