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
	private:
		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;
		b2Body* body;
		b2Shape* shape;
	};
}