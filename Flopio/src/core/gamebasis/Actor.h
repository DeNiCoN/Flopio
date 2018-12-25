#pragma once
#include <vector>
#include "../utils.h"
#include <memory>
#include "RenderComponent.h"
#include "../math/linearmath.h"

namespace engine
{

	class Component;
	class RenderComponent;

	using SharedComponent = std::shared_ptr<Component>;
	class Component
	{
	public:
		virtual void update(double delta) {}
		virtual const char* getName() const = 0;
		constexpr unsigned int getId() const { return hash(getName(), strlen(getName())); }

	};

	class Actor;
	typedef std::shared_ptr<Actor> SharedActor;
	class Actor
	{
	private:
		std::vector<SharedComponent> components;
		vec3 position;
		vec3 speed;
	public:
		std::shared_ptr<RenderComponent> renderer;
		std::vector<SharedComponent> getComponents() const { return components; }

		std::shared_ptr<RenderComponent> getRenderer() const { return renderer; }
	};
}