#include <string>
#include "Resource.h"
#include "../Engine.h"
#include <cctype>
#include <algorithm>

namespace engine
{
	Resource::Resource(const std::string &resName)
	{
		name = resName;
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);

		size_t pos = name.find(':', 0);
		if (pos == std::string::npos)
		{
			log << "Resource " << name << " don't contains ':'\n";
			colonPos = 0;
		}
		else
		{
			colonPos = static_cast<unsigned char>(pos);
		}

	}
}