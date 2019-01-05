#include "Scene.h"

namespace engine
{

	std::unordered_map<unsigned int, std::vector<SharedActor>> Scene::tempActorsMap;
	std::unordered_map<unsigned int, RenderFuntion> Scene::renderFunctionsMap;

	Camera::Camera()
	{
		view = mat44Identity(1.0);
	}

	void Camera::set(vec2 position, float angle, float scale)
	{
		this->position = position;
		this->angle = angle;
		this->scale = scale;
		updateView();
	}

	void Camera::updateView()
	{
		mat44 rotate, translate;
		translate = mat44TranslateVec2(vec2Neg(position));
		rotate = mat44RotateByZ(-angle);
		view = mat44Multiply(mat44Multiply(rotate, mat44Scale(scale, scale, 1.0f)), translate);
	}

	void Scene::render(const double ndelay)
	{
		//sort by renderer component id
		for (SharedActor actor : actors)
		{
			tempActorsMap[actor->getRenderer()->getId()].push_back(actor);
		}

		projectionView = mat44Multiply(viewport->getProjection(), camera.getView());

		//render   
		for (auto pair : tempActorsMap)
		{
			(*renderFunctionsMap[pair.first])(pair.second, *this, ndelay);
		}

		//clean
		for (auto pair : tempActorsMap)
		{
			pair.second.clear();
		}
	
	}

	void Scene::registerRenderer(unsigned int componentId, RenderFuntion renderFunction)
	{
		renderFunctionsMap[componentId] = renderFunction;
		tempActorsMap[componentId] = std::vector<SharedActor>();
	}

}