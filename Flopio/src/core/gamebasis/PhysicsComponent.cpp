#include "PhysicsComponent.h"

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

}
