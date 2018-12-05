#include "Scene.h"

namespace engine
{

	void Scene::render(const double ndelay)
	{
		for (SharedActor actor : actors)
		{
			actor->renderer->VRender(ndelay);
		}
	}

}