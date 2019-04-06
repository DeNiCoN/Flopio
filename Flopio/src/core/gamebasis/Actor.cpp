#include "Actor.h"

namespace engine
{



	std::vector<unsigned int> Component::renderIds;

	void Actor::setPosition(vec3 pos)
	{
		position = pos;
		renderer->VOnActorPositionSet(pos);
	}

	void Actor::setAngle(float radians)
	{
		angle = radians;
		renderer->VOnActorAngleSet(radians);
	}

	void Actor::setRenderer(std::shared_ptr<RenderComponent> renderer)
	{
		this->renderer = renderer;
		renderer->setParent(this);
	}


	void Actor::addComponent(SharedComponent component)
	{
		components.push_back(component);
		component->setParent(this);
	}

	void Actor::removeComponent(SharedComponent component)
	{
		auto t = std::find(components.begin(), components.end(), component);
		if (t != components.end())
		{
			components.erase(t);
			component->setParent(nullptr);
		}
	}

}