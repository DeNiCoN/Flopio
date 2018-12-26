#include "Scene.h"

namespace engine
{

	std::unordered_map<unsigned int, std::vector<SharedActor>> Scene::tempActorsMap;
	std::unordered_map<unsigned int, void(*)(std::vector<SharedActor>&, const double)> Scene::renderFunctionsMap;

	void Scene::render(const double ndelay)
	{
		//sort by renderer component id
		for (SharedActor actor : actors)
		{
			tempActorsMap[actor->getRenderer()->getId()].push_back(actor);
		}

		//render
		for (auto pair : tempActorsMap)
		{
			(*renderFunctionsMap[pair.first])(pair.second, ndelay);
		}

		//clean
		for (auto pair : tempActorsMap)
		{
			pair.second.clear();
		}
	
	}

	void Scene::registerRenderer(unsigned int componentId, void(*renderFunction)(std::vector<SharedActor>&, const double))
	{
		renderFunctionsMap[componentId] = renderFunction;
		tempActorsMap[componentId] = std::vector<SharedActor>();
	}

}