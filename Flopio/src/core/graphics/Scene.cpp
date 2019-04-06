#include "Scene.h"

namespace engine
{

	std::unordered_map<unsigned int, std::vector<SharedActor>> Scene::tempActorsMap;
	std::unordered_map<unsigned int, RenderFuntion> Scene::renderFunctionsMap;
	std::vector<std::shared_ptr<RenderComponent>> renderers;

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
		view = mat44Multiply(&mat44Multiply(&rotate, &mat44Scale(scale, scale, 1.0f)), &translate);
	}

	void Scene::render(const double ndelay)
	{
		//render   
		for (auto pair : tempActorsMap)
		{
			(*renderFunctionsMap[pair.first])(pair.second, *this, ndelay);
		}
	
	}

	void Scene::update(const double delta)
	{
		for (auto actor : actors)
		{
			actor->update(delta);
		}

		//update render data
		//clean
		tempActorsMap.clear();

		//sort by renderer component id
		for (SharedActor actor : actors)
		{
			tempActorsMap[actor->getRenderer()->getId()].push_back(actor);
		}

		projectionView = mat44Multiply(viewport->getProjection(), camera.getView());


	}

	void Scene::addActor(SharedActor actor)
	{
		actors.push_back(actor);
	}

	void Scene::registerRenderer(unsigned int componentId, RenderFuntion renderFunction)
	{
		renderFunctionsMap[componentId] = renderFunction;
		tempActorsMap[componentId] = std::vector<SharedActor>();
	}

}