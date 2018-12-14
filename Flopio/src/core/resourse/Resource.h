#pragma once
#include <string>

namespace engine
{
	class Resource
	{
	private:
		std::string name;
		unsigned int hashed;
		unsigned char colonPos;
	public:
		Resource(const std::string& resName);
		std::string getName() const { return name; };
		unsigned char getColonPos() const { return colonPos; };
		bool operator==(Resource a) { return hashed == a.hashed; }
	};
}