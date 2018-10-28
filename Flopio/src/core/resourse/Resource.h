#pragma once
#include <string>

namespace engine
{
	class Resource
	{
	public:
		std::string name;
		Resource(const std::string& resName);
	};
}