#include "ResourceCache.h"
#include "../utils.h"
#include "../Engine.h"
#include <iostream>

namespace engine {

	ResourceCache::~ResourceCache() 
	{
		if (cacheBuffer)
			delete cacheBuffer;
	}

	bool ResourceCache::init()
	{
		cacheBuffer = new char[cacheBufferSize];

		if (!cacheBuffer)
			return false;

		char* poolBuf = new char[maxHandleCount*sizeof(ResourceHandle)];
		if (!poolBuf)
			return false;
		if (!poolInit(&handlePool, poolBuf, sizeof(ResourceHandle), maxHandleCount))
			return false;

		//first data of cacheBuffer this is a pointer that points to first free location
		uintptr_t* ptrToUintptr = reinterpret_cast<uintptr_t*>(cacheBuffer) + 1;
		*reinterpret_cast<uintptr_t*>(cacheBuffer) = reinterpret_cast<uintptr_t>(ptrToUintptr);
		*ptrToUintptr = cacheBufferSize - sizeof(void*) * 2; // each free block contains it size

		return true;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::getHandle(Resource * resource)
	{
		std::shared_ptr<ResourceHandle> handle = find(resource);
		if (handle == nullptr)
			handle = load(resource);
		else
			update(handle);
		return handle;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::find(Resource * resource)
	{
		ResourceHandlesMap::iterator i = handlesMap.find(resource->getName());
		if (i == handlesMap.end())
			return std::shared_ptr<ResourceHandle>();
		return i->second;
	}

	void ResourceCache::update(std::shared_ptr<ResourceHandle> resHandle) 
	{
		LRUList.remove(resHandle);
		LRUList.push_front(resHandle);
	}

	char* ResourceCache::allocate(unsigned int size) 
	{ 
		char* ptr;
		unsigned int fullSize = size + sizeof(void*) * 2;
		uintptr_t* pastPtr = reinterpret_cast<uintptr_t*>(cacheBuffer);
		uintptr_t* nextPtr = reinterpret_cast<uintptr_t*>(*pastPtr);
		while (true)
		{
			unsigned int nextFreeSize = *nextPtr;
			if (fullSize <= nextFreeSize)
			{
				std::cout << fullSize << "::" << nextFreeSize << "\n";
				//enough space
				ptr = reinterpret_cast<char*>(nextPtr + 2);
				*nextPtr = fullSize;
				nextPtr = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(nextPtr) + fullSize);
				*pastPtr = reinterpret_cast<uintptr_t>(nextPtr);
				std::cout << (unsigned int)nextPtr << "::" << (unsigned int)(cacheBuffer + cacheBufferSize) << "::" << (unsigned int)(cacheBuffer + cacheBufferSize) -(unsigned int)nextPtr << "\n";
				*nextPtr = nextFreeSize - fullSize;
				return ptr;
			}
			pastPtr = reinterpret_cast<uintptr_t*>(reinterpret_cast<char*>(nextPtr) + nextFreeSize);
			if (reinterpret_cast<char*>(pastPtr) == (cacheBuffer + cacheBufferSize - sizeof(void*))) // out of buffer;
				break;
			uintptr_t* nextPtr = reinterpret_cast<uintptr_t*>(*pastPtr);
		}
		return nullptr; 
	}
	void ResourceCache::deallocate(char* buffer) {};

	std::shared_ptr<ResourceHandle> ResourceCache::load(Resource * resource) 
	{
		std::shared_ptr<ResourceHandle> handle;
		std::shared_ptr<ResourceLoader> loader;
		std::shared_ptr<ResourceFile> file;
		
		for (std::shared_ptr<ResourceLoader> l : loaders) 
		{
			if (wildcardMath(l->VGetWildcardPattern().c_str(), resource->getName().c_str()))
			{
				loader = l;
				break;
			}
		}

		if (!loader)
		{
			log << "Resource loader for " << resource->getName() << " not found\n";
			return handle;
		}

		for (std::shared_ptr<ResourceFile> f : files)
		{
			if (!resource->getName().compare(0, resource->getColonPos(), f->getName(), 0, resource->getColonPos()))
			{
				file = f;
				break;
			}
		}

		if (!file)
		{
			log << "ResourceFile for " << resource->getName() << " not found\n";
			return handle;
		}

		int rawSize = file->VGetRawResourceSize(*resource);
		if (rawSize < 0)
		{
			log << "Resource size returned -1 - Resource " << resource->getName() << " not found";
			return std::shared_ptr<ResourceHandle>();
		}
		char *rawBuffer = allocate(rawSize);

		if (rawBuffer == NULL || file->VGetRawResource(*resource, rawBuffer) == 0)
		{
			log << "ResourceCache out of memory while loading " << resource->getName() << "\n";
			return std::shared_ptr<ResourceHandle>();
		}

		char *buffer = NULL;
		unsigned int size = 0;

		if (loader->VUseRawFile())
		{
			buffer = rawBuffer;
			ResourceHandle * handl = reinterpret_cast<ResourceHandle *>(poolAlloc(&handlePool));
			*handl = ResourceHandle(*resource, buffer, rawSize, this);
			handle = std::shared_ptr<ResourceHandle>(handl);
		}
		else
		{
			size = loader->VGetLoadedSize(rawBuffer, rawSize) + loader->VGetExtraDataSize();
			buffer = allocate(size);
			if (rawBuffer == NULL || buffer == NULL)
			{
				log << "ResourceCache out of memory while loading " << resource->getName() << "\n";
				return std::shared_ptr<ResourceHandle>();
			}
			ResourceHandle * handl = reinterpret_cast<ResourceHandle *>(poolAlloc(&handlePool));
			*handl = ResourceHandle(*resource, buffer, rawSize, this);
			handle = std::shared_ptr<ResourceHandle>(handl);
			bool success = loader->VLoad(rawBuffer, rawSize, handle);

			if (loader->VDiscardRawBufferAfterLoad())
			{
				deallocate(rawBuffer);
			}

			if (!success)
			{
				log << "ResourceCache out of memory while loading " << resource->getName() << "\n";
				return std::shared_ptr<ResourceHandle>();
			}
		}

		if (handle)
		{
			LRUList.push_front(handle);
			handlesMap[resource->getName()] = handle;
		}

		return handle;

	}
}