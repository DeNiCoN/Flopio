#include "DirectoryResourceFile.h"
#include <filesystem>
#include <stdio.h>
#include "../../utils.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace engine
{

	bool DirectoryResourceFile::VOpen()
	{
		if (std::filesystem::exists(name))
		{
			if (!std::filesystem::is_directory(name))
			{
				return false;
			}
			int i = 0;
			for (auto p : std::filesystem::directory_iterator(name))
			{
				if (!p.is_directory())
				{
					std::string path = p.path().filename().string();
					std::transform(path.begin(), path.end(), path.begin(), (int(*)(int)) std::tolower);
					filesMap[path] = p.file_size();
				}
			}
			return true;
		}
 		return false;
	}

	void DirectoryResourceFile::VReload()
	{
		for (auto p : std::filesystem::directory_iterator(name))
		{
			if (!p.is_directory())
			{
				std::string path = p.path().filename().string();
				std::transform(path.begin(), path.end(), path.begin(), (int(*)(int)) std::tolower);
				filesMap[path] = p.file_size();
			}
		}
	}

	int DirectoryResourceFile::VGetRawResourceSize(const Resource &r) 
	{
		return filesMap[r.getName().substr(r.getSeparatorPos() + 1)];
	}

	int DirectoryResourceFile::VGetRawResource(const Resource &r, char *buffer)
	{
		std::ifstream file;
		file.open(name + "/" + r.getName().substr(r.getSeparatorPos() + 1), std::ifstream::in | std::ifstream::binary);
		if (file.fail())
			return -1;
		file.read(buffer, filesMap[r.getName().substr(r.getSeparatorPos() + 1)]);
		return 0;
	}

	int DirectoryResourceFile::VGetNumResources() const
	{
		return filesMap.size();
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
