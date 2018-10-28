#pragma once
#include <list>
#include <memory>
#include "Resource.h"
#include "ResourceHandle.h"
#include "ResourceLoader.h"
#include <map>
#include <string>
#include <vector>

namespace engine 
{
	typedef std::list<std::shared_ptr<ResourceHandle>> ResourceHandlesList;
	typedef std::map<std::string, std::shared_ptr<ResourceHandle>> ResourceHandlesMap;
	typedef std::vector<std::shared_ptr<ResourceLoader>> ResourceLoadersVector;

	class ResourceCache 
	{
	protected:
		char* cacheBuffer;
		unsigned int cacheBufferSize;
		unsigned int allocatedSize;

		ResourceHandlesList LRUList;
		ResourceHandlesMap handlesMap;
		ResourceLoadersVector loaders;

		char* allocate(unsigned int size);
		void makeRoom(unsigned int size);
		void update(std::shared_ptr<ResourceHandle> resHandle);
		void free(std::shared_ptr<ResourceHandle> resHandle);

	public:
		ResourceCache(unsigned int sizeInMB);
		~ResourceCache();
		bool init();
		std::shared_ptr<ResourceHandle> getHandle(Resource * resource);

	};
}