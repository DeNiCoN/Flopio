#pragma once
#include <vector>
#include "Component.h"
#include <memory>
#include "RenderComponent.h"
#include "../math/linearmath.h"

namespace engine
{
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