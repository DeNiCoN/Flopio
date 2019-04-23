#pragma once
#include "../ResourceFile.h"
#include <string>
#include <map>
#include <utility>

namespace engine
{
	class DirectoryResourceFile : public ResourceFile
	{
	private:
		std::map<std::string, unsigned int> filesMap;
	public:
		DirectoryResourceFile(std::string path) : ResourceFile(path) {}
		virtual void VReload() override;
		virtual bool VOpen();
		virtual int VGetRawResourceSize(const Resource &r);
		virtual int VGetRawResource(const Resource &r, char *buffer);
		virtual int VGetNumResources() const;
		virtual std::string VGetResourceName(int num) const;
	};
}