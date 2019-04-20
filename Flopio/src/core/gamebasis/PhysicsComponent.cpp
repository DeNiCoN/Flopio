#include "PhysicsComponent.h"
#include "../graphics/Scene.h"

namespace engine
{
	const char* PhysicsComponent::name = "PhysicsComponent";

	PhysicsComponent::~PhysicsComponent()
	{
		for (auto fixture : fixtureDefs)
		{
			delete &fixture.shape;
			delete &fixture;
		}
	}

	const char* PhysicsComponent::getName() const
	{
		return name;
	}

	bool PhysicsComponent::VInit(const tinyxml2::XMLElement* pData)
	{
		if (pData->NoChildren())
			return false;
		bodyDef.position.Set(pData->FloatAttribute("position.x"), pData->FloatAttribute("position.y"));
		bodyDef.angle = pData->FloatAttribute("angle");
		bodyDef.angularVelocity = pData->FloatAttribute("angularVelocity");
		bodyDef.linearDamping = pData->FloatAttribute("linearDamping");
		bodyDef.linearVelocity.Set(pData->FloatAttribute("velocity.x"), pData->FloatAttribute("velocity.y"));
		bodyDef.angularDamping = pData->FloatAttribute("angularDamping");
		bodyDef.bullet = pData->BoolAttribute("bullet");
		const char* type = pData->Attribute("type");
		if (strcmp(type, "dynamic"))
		{
			bodyDef.type = b2_dynamicBody;
		} 
		else if (strcmp(type, "kinematic"))
		{
			bodyDef.type = b2_kinematicBody;
		}
		else 
		{
			bodyDef.type = b2_staticBody;
		}

		for (const tinyxml2::XMLElement* pNode = pData->FirstChildElement("Fixture"); pNode; pNode = pNode->NextSiblingElement("Fixture"))
		{
			if (pNode->NoChildren())
				continue;

			b2FixtureDef* f = new b2FixtureDef();
			f->density = pNode->FloatAttribute("density");
			f->friction = pNode->FloatAttribute("friction");
			f->restitution = pNode->FloatAttribute("restitution");
			f->filter.categoryBits = pNode->IntAttribute("categoryBits");
			f->filter.groupIndex = pNode->IntAttribute("groupIndex");
			f->filter.maskBits = pNode->IntAttribute("maskBits");

			if (auto pNode1 = pNode->FirstChildElement("Box"))
			{
				b2PolygonShape* shape = new b2PolygonShape();
				shape->SetAsBox(pNode1->FloatAttribute("width") / 2,
					pNode1->FloatAttribute("heigth") / 2,
					{ pNode1->FloatAttribute("x"),
					pNode1->FloatAttribute("y") },
					pNode1->FloatAttribute("angle"));
				f->shape = shape;
			}
			else if (auto pNode1 = pNode->FirstChildElement("Circle"))
			{
				b2CircleShape* shape = new b2CircleShape();
				shape->m_radius = pNode1->FloatAttribute("radius");
				shape->m_p.x = pNode1->FloatAttribute("x");
				shape->m_p.y = pNode1->FloatAttribute("y");
				f->shape = shape;
			}
			else if (auto pNode1 = pNode->FirstChildElement("Polygon"))
			{
				if (pNode->NoChildren())
					continue;
				b2PolygonShape* shape = new b2PolygonShape();
				int n = 0;
				for (const tinyxml2::XMLElement* pNode2 = pNode1->FirstChildElement("Point"); pNode2; pNode2 = pNode2->NextSiblingElement("Point"))
				{
					n++;
				}
				b2Vec2* vertices = new b2Vec2[n];
				int i = 0;
				for (const tinyxml2::XMLElement* pNode2 = pNode1->FirstChildElement("Point"); pNode2; pNode2 = pNode2->NextSiblingElement("Point"))
				{
					vertices[i++] = { pNode2->FloatAttribute("x"), pNode2->FloatAttribute("y") };
				}
				shape->Set(vertices, n);
				delete[] vertices;

				f->shape = shape;
			}
			else if (auto pNode1 = pNode->FirstChildElement("Chain"))
			{
				if (pNode->NoChildren())
					continue;
				b2ChainShape* shape = new b2ChainShape();
				int n = 0;
				for (const tinyxml2::XMLElement* pNode2 = pNode1->FirstChildElement("Point"); pNode2; pNode2 = pNode2->NextSiblingElement("Point"))
				{
					n++;
				}
				b2Vec2* vertices = new b2Vec2[n];
				int i = 0;
				for (const tinyxml2::XMLElement* pNode2 = pNode1->FirstChildElement("Point"); pNode2; pNode2 = pNode2->NextSiblingElement("Point"))
				{
					vertices[i++] = { pNode2->FloatAttribute("x"), pNode2->FloatAttribute("y") };
				}
				shape->CreateChain(vertices, n);
				delete[] vertices;

				f->shape = shape;
			}
			else if (auto pNode1 = pNode->FirstChildElement("Loop"))
			{
				if (pNode->NoChildren())
					continue;
				b2ChainShape* shape = new b2ChainShape();
				int n = 0;
				for (const tinyxml2::XMLElement* pNode2 = pNode1->FirstChildElement("Point"); pNode2; pNode2 = pNode2->NextSiblingElement("Point"))
				{
					n++;
				}
				b2Vec2* vertices = new b2Vec2[n];
				int i = 0;
				for (const tinyxml2::XMLElement* pNode2 = pNode1->FirstChildElement("Point"); pNode2; pNode2 = pNode2->NextSiblingElement("Point"))
				{
					vertices[i++] = { pNode2->FloatAttribute("x"), pNode2->FloatAttribute("y") };
				}
				shape->CreateLoop(vertices, n);
				delete[] vertices;

				f->shape = shape;
			}
			else
				continue;

			fixtureDefs.push_back(*f);
		}

		if (fixtureDefs.size() == 0)
			return false;
		return true;
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
		scene.getWorld().DestroyBody(body);
	}

}
