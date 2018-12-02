#pragma once
#include <vector>
#include "Actor.h"

namespace engine
{
	class Scene
	{
	public:
		void onRender(const double ndelay);
		void addActor(SharedActor actor);
	private:
		std::vector<SharedActor> actors;
	};
}