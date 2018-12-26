#pragma once
#include <vector>
#include "../utils.h"
#include <memory>
#include "../math/linearmath.h"

namespace engine
{

	class Component
	{
	public:
		virtual void update(double delta) {}
		virtual const char* getName() const = 0;
		constexpr unsigned int getId() const { return hash(getName(), strlen(getName())); }

	};

	using SharedComponent = std::shared_ptr<Component>;
	class RenderComponent;

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
	typedef std::shared_ptr<Actor> SharedActor;
}