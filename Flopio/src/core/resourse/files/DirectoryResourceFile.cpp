#include "DirectoryResourceFile.h"
#include <filesystem>
#include <stdio.h>
#include "../../utils.h"

namespace engine
{
	DirectoryResourceFile::DirectoryResourceFile(std::string path)
	{
		name = path;
	}

	bool DirectoryResourceFile::VOpen()
	{
		int i = 0;
		for (auto p : std::filesystem::directory_iterator(name))
		{
			if (!p.is_directory())
			{
				filesMap[splitpath(p.path, { '/' }).back()] = p.file_size();
			}
		}
	}

	int DirectoryResourceFile::VGetRawResourceSize(const Resource &r) 
	{
		return filesMap[r.getName().substr(r.getColonPos() + 1)];
	}

	int DirectoryResourceFile::VGetRawResource(const Resource &r, char *buffer)
	{
		
	}

	std::string DirectoryResourceFile::VGetResourceName(int num) const
	{
		int i = 0;
		for(auto d : filesMap)
		{
			if (i++ == num)
			{
				return d.first;
			}
		}
		return nullptr;
	}
}