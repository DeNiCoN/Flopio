#include <string>
#include "Resource.h"
#include "../Engine.h"
#include <cctype>
#include <algorithm>
#include "../utils/utils.h"

namespace engine
{
	const char Resource::separator = '|';
	Resource::Resource()
	{
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		hashed = hash(name.c_str(), name.length());
		size_t pos = name.find(separator, 0);
		if (pos == std::string::npos)
		{
			logger << "Resource " << name << " don't contains " << "'" << separator << "'\n";
			separatorPos = 0;
		}
		else
		{
			separatorPos = static_cast<unsigned char>(pos);
		}
	}

	Resource::Resource(const std::string &resName)
	{
		name = resName;
		hashed = hash(name.c_str(), name.length());
		size_t pos = name.find(separator, 0);
		if (pos == std::string::npos)
		{
			logger << "Resource " << name << " don't contains "<< "'" << separator << "'\n";
			separatorPos = 0;
		}
		else
		{
			separatorPos = static_cast<unsigned char>(pos);
		}
	}

	/*Resource::Resource(const char* resName)
	{
		name = std::string(resName);
		Resource();
	}*/
}
