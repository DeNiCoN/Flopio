#pragma once
#include "../ResourceFile.h"
#include <string>
#include <map>
#include <utility>

namespace engine
{
	class DirectoryResourceFile : ResourceFile
	{
	private:
		std::map<std::string, unsigned int> filesMap;
	public:
		DirectoryResourceFile(std::string path);
		virtual bool VOpen();
		virtual int VGetRawResourceSize(const Resource &r);
		virtual int VGetRawResource(const Resource &r, char *buffer);
		virtual int VGetNumResources() const = 0;
		virtual std::string VGetResourceName(int num) const;
	};
}