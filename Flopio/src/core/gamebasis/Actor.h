#pragma once
#include <vector>
#include "Component.h"
#include <memory>

namespace engine
{
	typedef std::shared_ptr<Actor> SharedActor;
	class Actor
	{
		std::vector<Component> components;
	};
}