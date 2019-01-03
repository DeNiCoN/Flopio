#pragma once
#include "Viewport.h"

namespace engine
{
	class ScreenViewport : public Viewport
	{
	public:
		virtual void VResize(int width, int height) override;
	};
}