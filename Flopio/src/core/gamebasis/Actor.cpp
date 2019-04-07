#include "Actor.h"

namespace engine
{



	std::vector<unsigned int> Component::renderIds;

	void Actor::setPosition(vec3 pos)
	{
		position = pos;
		for (auto comp : components)
		{
			comp->VOnActorPositionSet(pos);
		}
	}

	void Actor::setAngle(float radians)
	{
		angle = radians;
		for (auto comp : components)
		{
			comp->VOnActorAngleSet(radians);
		}
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