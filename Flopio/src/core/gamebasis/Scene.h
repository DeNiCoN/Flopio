#pragma once
#include <vector>
#include "Actor.h"
#include "RenderComponent.h"
#include <unordered_map>

namespace engine
{
	class Scene
	{
	public:
		void render(const double ndelay);
		void addActor(SharedActor actor) { actors.push_back(actor); }
		static void registerRenderer(unsigned int componentId, void(*renderFunction)(std::vector<SharedActor>&, const double));
	private:
		std::vector<SharedActor> actors;
		static std::unordered_map<unsigned int, std::vector<SharedActor>> tempActorsMap;
		static std::unordered_map<unsigned int, void(*)(std::vector<SharedActor>&, const double)> renderFunctionsMap;
	};
}