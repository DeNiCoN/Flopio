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
		virtual bool VOpen();
		virtual int VGetRawResourceSize(const Resource &r);
		virtual int VGetRawResource(const Resource &r, char *buffer);
		virtual int VGetNumResources() const;
		virtual std::string VGetResourceName(int num) const;

		std::string getName() const { return name; };
	};
}