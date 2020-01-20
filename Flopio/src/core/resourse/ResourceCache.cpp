#include "ResourceCache.h"
#include "files/DirectoryResourceFile.h"
#include "../utils/utils.h"
#include "../Engine.h"
#include <iostream>

namespace engine {

	ResourceCache::~ResourceCache() 
	{
		LRUList.clear();
		handlesMap.clear();
		files.clear();
		loaders.clear();
	}

	DirectoryResourceFile engineResFile("EngineResources");

	bool ResourceCache::init()
	{
		char* poolBuf = new char[maxHandleCount*sizeof(ResourceHandle)];
		if (!poolBuf)
			return false;
		if (!poolInit(&handlePool, poolBuf, sizeof(ResourceHandle), maxHandleCount))
			return false;

		addFile(std::shared_ptr<DirectoryResourceFile>(&engineResFile, [](DirectoryResourceFile*) {}));

		return true;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::getHandle(Resource & resource)
	{
		std::shared_ptr<ResourceHandle> handle = find(resource);
		if (handle == nullptr)
		{
			handle = load(resource);
		}
		else
		{
			//update(handle);
		}
		return handle;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::find(Resource & resource)
	{
		ResourceHandlesMap::iterator i = handlesMap.find(resource.getHashed());
		if (i == handlesMap.end())
			return std::shared_ptr<ResourceHandle>();
		return i->second;
	}

	void ResourceCache::update(std::shared_ptr<ResourceHandle> resHandle) 
	{
		LRUList.remove(resHandle);
		LRUList.push_front(resHandle);
	}

	

	void ResourceCache::makeRoom(unsigned int size)
	{
		//lol
	}

	std::shared_ptr<ResourceHandle> ResourceCache::load(Resource & resource) 
	{
		std::shared_ptr<ResourceHandle> handle;
		std::shared_ptr<ResourceLoader> loader;
		std::shared_ptr<ResourceFile> file;
		//std::cout << "load " << resource->getName() << "\n";
		for (std::shared_ptr<ResourceLoader> l : loaders) 
		{
			int size;
			const std::string* wilds = l->VGetWildcardPattern(&size);
			for (int i = 0; i < size; i++)
			{
				if (wildcardMath(wilds[i].c_str(), resource.getName().substr(resource.getSeparatorPos() + 1).c_str()))
				{
					loader = l;
					break;
				}
			}
			if (loader)
				break;
		}

		if (!loader)
		{
			logger << "Resource loader for " << resource.getName() << " not found\n";
			return handle;
		}

		for (std::shared_ptr<ResourceFile> f : files)
		{
			if (!resource.getName().compare(0, resource.getSeparatorPos(), f->getName(), 0, resource.getSeparatorPos()))
			{
				file = f;
				break;
			}
		}

		if (!file)
		{
			logger << "ResourceFile for " << resource.getName() << " not found\n";
			return handle;
		}

		int rawSize = file->VGetRawResourceSize(resource);
		if (rawSize == 0)
		{
			logger << "Resource size returned 0 - Resource " << resource.getName() << " not found\n";
			return std::shared_ptr<ResourceHandle>();
		}
		char *rawBuffer = new char[rawSize + 1];
		rawBuffer[rawSize] = '\0';

		if (rawBuffer == NULL)
		{
			logger << "ResourceCache out of memory while loading " << resource.getName() << "\n";
			return std::shared_ptr<ResourceHandle>();
		}

		if (file->VGetRawResource(resource, rawBuffer) == -1)
		{
			logger << "Something went wrong when getting " << resource.getName() << "\n";
			return std::shared_ptr<ResourceHandle>();
		}

		char *buffer = NULL;
		unsigned int size = 0;

		if (loader->VUseRawFile())
		{
			buffer = rawBuffer;
			ResourceHandle * handl = reinterpret_cast<ResourceHandle *>(poolAlloc(&handlePool));
			if (!handl)
			{
				logger << "ResourceCache out of ResourceHandles\n";
				return std::shared_ptr<ResourceHandle>();
			}
			new (handl) ResourceHandle(resource, buffer, rawSize, this);
			handle = std::shared_ptr<ResourceHandle>(handl,
			[&](ResourceHandle * ptr)
			{
				delete[](ptr->getBuffer());
				poolFree(&this->handlePool, ptr);
			});
		}
		else
		{
			size = loader->VGetLoadedSize(rawBuffer, rawSize) + loader->VGetExtraDataSize();
			buffer = new char[size];
			if ((rawBuffer == NULL || buffer == NULL) && size != 0)
			{
				logger << "ResourceCache out of memory while loading " << resource.getName() << "\n";
				return std::shared_ptr<ResourceHandle>();
			}
			handle = std::shared_ptr<ResourceHandle>(new (reinterpret_cast<ResourceHandle *>(poolAlloc(&handlePool))) ResourceHandle(resource, buffer, size, this),
			[&](ResourceHandle * ptr)
			{
				delete[](ptr->getBuffer());
				poolFree(&this->handlePool, ptr);
			});
			bool success = loader->VLoad(rawBuffer, rawSize, handle);

			if (loader->VDiscardRawBufferAfterLoad())
			{
				delete[](rawBuffer);
			}

			if (!success)
			{
				logger << "Something went wrong when loading " << resource.getName() << "\n";
				return std::shared_ptr<ResourceHandle>();
			}
		}

		if (handle)
		{
			LRUList.push_front(handle);
			handlesMap[resource.getHashed()] = handle;
		}

		return handle;

	}

	void ResourceCache::addFile(std::shared_ptr<ResourceFile> file) 
	{
		if(std::find(files.begin(), files.end(), file) == files.end())
			if (file->VOpen())
				files.push_back(file);
			else
				logger << "failed to open file " << file->getName() << "\n";
	}

	void ResourceCache::clearResources()
	{
		handlesMap.clear();
		for (auto file : files)
		{
			file->VReload();
		}
	}
}
