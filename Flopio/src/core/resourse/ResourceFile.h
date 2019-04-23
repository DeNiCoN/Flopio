#pragma once
#include "Resource.h"
#include <algorithm>
#include <cctype>

namespace engine
{
	class ResourceFile 
	{
	protected:
		std::string name;
	public:
		ResourceFile(std::string fileName) 
		{
			name = fileName;
			std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		}
		virtual ~ResourceFile() {};
		virtual void VReload() = 0;
		virtual bool VOpen() = 0;
		virtual int VGetRawResourceSize(const Resource &r) = 0;
		virtual int VGetRawResource(const Resource &r, char *buffer) = 0;
		virtual int VGetNumResources() const = 0;
		virtual std::string VGetResourceName(int num) const = 0;

		std::string getName() const { return name; };
	};
}