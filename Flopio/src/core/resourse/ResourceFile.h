#pragma once
#include "Resource.h"

namespace engine
{
	class ResourceFile 
	{
	protected:
		std::string name;
	public:
		virtual ~ResourceFile() {};
		virtual bool VOpen() = 0;
		virtual int VGetRawResourceSize(const Resource &r) = 0;
		virtual int VGetRawResource(const Resource &r, char *buffer) = 0;
		virtual int VGetNumResources() const = 0;
		virtual std::string VGetResourceName(int num) const = 0;

		std::string getName() const { return name; };
	};
}