#include <string>
#include "Resource.h"
#include "../Engine.h"
#include <cctype>
#include <algorithm>
#include "../utils.h"

namespace engine
{
	Resource::Resource()
	{
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		hashed = hash(name.c_str(), name.length());
		size_t pos = name.find(':', 0);
		if (pos == std::string::npos)
		{
			logger << "Resource " << name << " don't contains ':'\n";
			colonPos = 0;
		}
		else
		{
			colonPos = static_cast<unsigned char>(pos);
		}
	}

	Resource::Resource(const std::string &resName)
	{
		name = resName;
		Resource();
	}

	/*Resource::Resource(const char* resName)
	{
		name = std::string(resName);
		Resource();
	}*/
}