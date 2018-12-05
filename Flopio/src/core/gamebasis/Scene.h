#pragma once
#include <vector>
#include "Actor.h"

namespace engine
{
	class Scene
	{
	public:
		void render(const double ndelay);
		void addActor(SharedActor actor) { actors.push_back(actor); }
	private:
		std::vector<SharedActor> actors;
	};
}