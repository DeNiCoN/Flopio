#include "DirectoryResourceFile.h"
#include <filesystem>
#include <stdio.h>
#include "../../utils.h"
#include <fstream>

namespace engine
{
	DirectoryResourceFile::DirectoryResourceFile(std::string path)
	{
		name = path;
	}

	bool DirectoryResourceFile::VOpen()
	{
		if (std::filesystem::exists(name))
		{
			if (!std::filesystem::is_directory(name))
				return false;
			int i = 0;
			for (auto p : std::filesystem::directory_iterator(name))
			{
				if (!p.is_directory())
				{
					filesMap[splitpath(p.path, { '/' }).back()] = p.file_size();
				}
			}
			return true;
		}
		return false;
	}

	int DirectoryResourceFile::VGetRawResourceSize(const Resource &r) 
	{
		return filesMap[r.getName().substr(r.getColonPos() + 1)];
	}

	int DirectoryResourceFile::VGetRawResource(const Resource &r, char *buffer)
	{
		std::fstream file;
		file.open(name + r.getName().substr(r.getColonPos + 1), std::fstream::in);
		if (file.fail())
			return -1;
		file.read(buffer, filesMap[r.getName()]);
		return 0;
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