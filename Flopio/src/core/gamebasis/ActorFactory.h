#pragma once
#include "Actor.h"
#include <tinyxml2.h>

namespace engine
{

	template <typename T>
	Component* createComponent() {
		return new T();
	}

	class ActorFactory
	{
	private:
		std::unordered_map<unsigned int, Component* *()> genericComponentFactory;
	public:
		template <typename T>
		void registerComponent(unsigned int id) {}

		SharedActor createActor(Resource xmlFile, const vec3* initialPosition = nullptr, const float* initialAngle = nullptr);
		void modifyActor(SharedActor actor, tinyxml2::XMLElement* data);

		SharedComponent VCreateComponent(tinyxml2::XMLElement* data);

		static unsigned int nextActorId() { static unsigned int next = 1; return next++; }
	};
}