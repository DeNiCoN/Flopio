#pragma once

namespace engine
{
	class RenderComponent
	{
		virtual void VRender(const double ndelay) = 0;
	};
}