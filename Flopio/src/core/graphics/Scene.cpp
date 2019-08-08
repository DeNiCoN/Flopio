#include "Scene.h"

namespace engine
{
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

	Scene::~Scene()
	{
		actors.clear();
		renderers.clear();
	}

	void Scene::render(const double ndelay)
	{
		//render   
		for (auto pair : tempRenderersMap)
		{
			(*renderFunctionsMap[pair.first])(pair.second, *this, ndelay);
		}
	
	}

	void Scene::update(const double delta)
	{
		world.Step(delta, 8, 3);

		for (auto actor : actors)
		{
			actor->update(delta);
		}

		//update render data
		//clean
		tempRenderersMap.clear();

		//sort by renderer component id
		for (auto rendComp : renderers)
		{
			if (rendComp->hasParent())
			{
				tempRenderersMap[rendComp->getId()].push_back(rendComp);
			}
			else
			{
				rendererRemoved(rendComp);
			}
		}
		projectionView = mat44Multiply(viewport->getProjection(), camera.getView());

	}

	void Scene::addActor(SharedActor actor)
	{
		actors.push_back(actor);
		for (auto component : actor->components)
		{
			if (Component::isRender(component->getId()))
			{
				renderers.push_back(std::static_pointer_cast<RenderComponent>(component));
			}
			component->VOnActorAddedToScene(*this);
		}
	}

	void Scene::removeActor(SharedActor actor)
	{
		actors.erase(std::find(actors.begin(), actors.end(), actor));
		for (auto component : actor->components)
		{
			if (Component::isRender(component->getId()))
			{
				rendererRemoved(std::static_pointer_cast<RenderComponent>(component));
			}
			component->VOnActorRemovedFromScene(*this);
		}
	}

	void Scene::registerRenderer(unsigned int componentId, RenderFuntion renderFunction)
	{
		renderFunctionsMap[componentId] = renderFunction;
	}

	void Scene::rendererRemoved(std::shared_ptr<RenderComponent> renderer)
	{
		renderers.erase(std::find(renderers.begin(), renderers.end(), renderer));
	}

}
