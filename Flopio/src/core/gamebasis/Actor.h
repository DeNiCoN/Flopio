#pragma once
#include <vector>
#include "Component.h"

namespace engine
{
	class Actor
	{
		std::vector<Component> components;
	};
}