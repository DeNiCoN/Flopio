#include "PhysicsComponent.h"
#include "../graphics/Scene.h"

namespace engine
{
	const char* PhysicsComponent::name = "PhysicsComponent";

	PhysicsComponent::~PhysicsComponent()
	{
		
	}

	const char* PhysicsComponent::getName() const
	{
		return name;
	}

	bool PhysicsComponent::VInit(const tinyxml2::XMLElement* pData)
	{
		return false;
	}

	void PhysicsComponent::VOnActorAddedToScene(Scene& scene)
	{
		body = scene.getWorld().CreateBody(&bodyDef);
		for (auto fixtureDef : fixtureDefs)
		{
			body->CreateFixture(&fixtureDef);
		}
	}

	void PhysicsComponent::VOnActorRemovedFromScene(Scene& scene)
	{

	}

}
