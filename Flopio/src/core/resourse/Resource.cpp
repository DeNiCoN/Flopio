#include <string>
#include "Resource.h"
#include <cctype>
#include <algorithm>

namespace engine
{
	Resource::Resource(const std::string &resName)
	{
		name = resName;
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
	}
}