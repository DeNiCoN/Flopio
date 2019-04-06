#pragma once
#include "../../math/linearmath.h"

namespace engine
{
	class Viewport
	{
	protected:
		mat44 projection;
	public:
		const mat44* getProjection() const { return &projection; }
		virtual void VResize(int width, int height) = 0;
		virtual ~Viewport() {};
	};
}
