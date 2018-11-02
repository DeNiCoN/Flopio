#pragma once
#include <list>
#include <memory>
#include "Resource.h"
#include "ResourceHandle.h"
#include "ResourceLoader.h"
#include <map>
#include <string>
#include <vector>
#include "../memory/poolAlloc.h"

namespace engine 
{
	typedef std::list<std::shared_ptr<ResourceHandle>> ResourceHandlesList;
	typedef std::map<std::string, std::shared_ptr<ResourceHandle>> ResourceHandlesMap;
	typedef std::vector<std::shared_ptr<ResourceLoader>> ResourceLoadersVector;
	typedef std::list<std::shared_ptr<ResourceHandle>> ResourceHandlesList;

	class ResourceCache 
	{
	protected:
		PoolAllocator handlePool;
		unsigned int maxHandleCount;

		char* cacheBuffer;
		unsigned int cacheBufferSize;
		unsigned int allocatedSize;

		ResourceHandlesList LRUList;
		ResourceHandlesMap handlesMap;
		ResourceLoadersVector loaders;

		char* allocate(unsigned int size);
		void makeRoom(unsigned int size);
		std::shared_ptr<ResourceHandle> load(Resource * resource);
		void update(std::shared_ptr<ResourceHandle> resHandle);
		void free(std::shared_ptr<ResourceHandle> resHandle);
		void freeOneResource();
		std::shared_ptr<ResourceHandle> find(Resource * resource);

	public:
		//setting default maxHandleCount to sizeInMB * 256 means that we 'have' 4 KB for each handle
		ResourceCache(unsigned int sizeInMB) : cacheBufferSize(sizeInMB * 1024 * 1024), allocatedSize(0), maxHandleCount(sizeInMB * 256) {}
		virtual ~ResourceCache();
		bool init();
		std::shared_ptr<ResourceHandle> getHandle(Resource * resource);

	};
}