#pragma once
#include <string>
#include <algorithm>

namespace engine
{

	class Resource
	{
	private:
		std::string name;
		unsigned int hashed;
		unsigned char colonPos;
		Resource();
	public:
		Resource(const std::string& resName);
		//Resource(const char* resName);
		std::string getName() const { return name; };
		unsigned char getColonPos() const { return colonPos; };
		bool operator==(Resource a) { return hashed == a.hashed; }
		unsigned int getHashed() const { return hashed; }
	};

	inline bool operator< (const Resource& lhs, const Resource& rhs) { return lhs.getHashed() < rhs.getHashed(); }

}