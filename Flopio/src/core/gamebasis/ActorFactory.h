#pragma once
#include "Actor.h"
#include <tinyxml2.h>

namespace engine
{

	template <typename T>
	Component* createComponentFunc() {
		return new T();
	}

	class ActorFactory
	{
	private:
		std::unordered_map<unsigned int, Component* *()> genericComponentFactory;
	public:
		template <typename T>
		void registerComponent(unsigned int id) { genericComponentFactory[id] = createComponentFunc<T>}

		ActorFactory();
		SharedActor createActor(const tinyxml2::XMLElement* pRoot, const vec3* initialPosition = nullptr, const float* initialAngle = nullptr);
		SharedActor createActor(Resource xmlFile, const vec3* initialPosition = nullptr, const float* initialAngle = nullptr);
		void modifyActor(SharedActor actor, tinyxml2::XMLElement* data);

		SharedComponent createComponent(const tinyxml2::XMLElement* data);

		static unsigned int nextActorId() { static unsigned int next = 1; return next++; }
	};
}