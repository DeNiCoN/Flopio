#pragma once
#include "Actor.h"
#include "../Box2D.h"

namespace engine
{
	class PhysicsComponent : public Component
	{
	public:
		static const char* name;
		PhysicsComponent() { id = Component::getId(name); }
		~PhysicsComponent();
		virtual const char* getName() const override;
		virtual bool VInit(const tinyxml2::XMLElement* pData) override;
		virtual void VOnActorAddedToScene(Scene& scene) override;
		virtual void VOnActorRemovedFromScene(Scene& scene) override;
	private:
		b2BodyDef bodyDef;
		std::vector<b2FixtureDef> fixtureDefs;
		b2Body* body;
	};
}