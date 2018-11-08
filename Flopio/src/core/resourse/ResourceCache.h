#pragma once
#include <list>
#include <memory>
#include "Resource.h"
#include "ResourceHandle.h"
#include "ResourceLoader.h"
#include "ResourceFile.h"
#include <map>
#include <string>
#include <vector>
#include "../memory/poolAlloc.h"

namespace engine 
{
	typedef std::list<std::shared_ptr<ResourceHandle>> ResourceHandlesList;
	typedef std::map<std::string, std::shared_ptr<ResourceHandle>> ResourceHandlesMap;
	typedef std::vector<std::shared_ptr<ResourceLoader>> ResourceLoadersVector;
	typedef std::vector<std::shared_ptr<ResourceFile>> ResourceFileVector;

	class ResourceCache 
	{
		friend class ResourceHandle;

	protected:
		PoolAllocator handlePool;
		unsigned int maxHandleCount;

		char* cacheBuffer;
		unsigned int cacheBufferSize;
		unsigned int allocatedSize;

		ResourceHandlesList LRUList;
		ResourceHandlesMap handlesMap;
		ResourceLoadersVector loaders;
		ResourceFileVector files;

		char* allocate(unsigned int size);
		void deallocate(char* buffer);
		void makeRoom(unsigned int size);
		std::shared_ptr<ResourceHandle> load(Resource * resource);
		void update(std::shared_ptr<ResourceHandle> resHandle);
		void free(std::shared_ptr<ResourceHandle> resHandle);
		void freeOneResource();
		std::shared_ptr<ResourceHandle> find(Resource * resource);

	public:
		//setting default maxHandleCount to sizeInMB * 256 means that we 'have' 4 KB of buffer for each handle
		ResourceCache(unsigned int sizeInMB) : cacheBufferSize(sizeInMB * 1024 * 1024), allocatedSize(0), maxHandleCount(sizeInMB * 256) {}
		virtual ~ResourceCache();
		bool init();
		bool init(unsigned int maxHandleCount) { this->maxHandleCount = maxHandleCount; return init(); }
		std::shared_ptr<ResourceHandle> getHandle(Resource * resource);
		void addLoader(std::shared_ptr<ResourceLoader> loader) { loaders.push_back(loader); }
		void addFile(std::shared_ptr<ResourceFile> file) { files.push_back(file); }
	};
}