#pragma once
#include <string>

namespace engine
{
	class Resource
	{
	private:
		std::string name;
		unsigned char colonPos;
	public:
		Resource(const std::string& resName);
		std::string getName() const { return name; };
		unsigned char getColonPos() const { return colonPos; };
	};
}